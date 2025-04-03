from typing import Any
from SCons.Script import Import  # type: ignore
Import("env")
env: Any  # Type hint ekle

import os
import re  # re modülünü ekliyoruz
import subprocess
from datetime import datetime  # Tarih ve saat bilgisi almak için gerekli
import configparser
import sys
import platform

# Log dosyasının yolu
log_file_path = 'pyScript/uplog.txt'
cihazListesi_file_path = 'pyScript/cihazListesi.txt'
rev_header_file_path = 'src/rev.h'

def get_git_version_and_date():
    try:
        # En son git etiketini al
        tag = subprocess.check_output(["git", "describe", "--tags"]).strip().decode('utf-8')
        # Etiketin tarihini al
        date = subprocess.check_output(
            ["git", "log", "-1", "--format=%ad", "--date=short", tag]).strip().decode('utf-8')
        # Version numaralarını ayır
        version_parts = tag.lstrip('v').split('-')[0].split('.')
        version_major = int(version_parts[0])
        version_minor = int(version_parts[1])
        version_patch = int(version_parts[2])
        return version_major, version_minor, version_patch, date
    except subprocess.CalledProcessError:
        return 0, 0, 1, "Unknown"

def update_rev_h(env):
    version_major, version_minor, version_patch, date = get_git_version_and_date()
    project_name = get_project_name()  # Proje adını alıyoruz
    build_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")

    header_content = f"""#include "header.h"
#define _VERSION_Project "{project_name}"
#define _VERSION_Major {version_major}
#define _VERSION_Minor {version_minor}
#define _VERSION_Patch {version_patch}
#define _VERSION_String "v{version_major}.{version_minor}.{version_patch}"
#define _VERSION_Date "{date}"
#define _VERSION_Build "{build_time}"
"""

    with open(rev_header_file_path, 'w') as file:
        file.write(header_content)

    print(f"rev.h dosyası güncellendi: {rev_header_file_path}")


#------------------------------------------------------------------------------#



def find_env_section(config, target_env):
    # `platformio.ini` dosyasındaki env:* bölümlerini bulur ve verilen hedef çevreyi kontrol eder.
    for section in config.sections():
        if section.startswith('env:') and target_env in section:
            return section
    return None

def get_default_env(config):
    # `platformio.ini` dosyasından varsayılan environment'ı alır
    return config['platformio'].get('default_envs', '').split(',')[0].strip()

def get_project_name():
    # `platformio.ini` dosyasını okuma
    config = configparser.ConfigParser()
    config.read('platformio.ini')

    # Öncelikle varsayılan environment'ı al
    target_env = get_default_env(config)
    if not target_env:
        target_env = os.getenv('PLATFORMIO_ENV', 'esp32-c3-devkitc-02')
    
    env_section = find_env_section(config, target_env)
    
    if env_section:
        return config[env_section].get('custom_project_name', 'UnknownProject')
    return 'UnknownProject'


def custom_upload_action(source, target, env):
    
    # İşletim sistemini kontrol et
    if platform.system() == 'Windows':
        print("    ::::Windowsta Script sonlandır.")
        return

    if os.path.exists(log_file_path):
        with open(log_file_path, 'r') as file:
            log_content = file.read()
        if '[SUCCESS]' in log_content:
            print("    ::::::::Log dosyasında [SUCCESS] bulundu.:::::")
            with open(log_file_path, 'w') as file_to_clear:
                file_to_clear.write('')
            command = f"platformio run --target upload | tee {log_file_path}"
            process = subprocess.Popen(command, shell=True)
            env.Exit(0)
                
        elif '[FAIL]' in log_content:
            print("    ::::::::Log dosyasında [FAIL] bulundu.:::::")
            with open(log_file_path, 'w') as file_to_clear:
                file_to_clear.write('')
                command = f"platformio run --target upload | tee {log_file_path}"
                process = subprocess.Popen(command, shell=True)
                env.Exit(0)
                
        else:
            print("    ::::::::Log dosyasında [SUCCESS] veya [FAIL] bulunamadı.:::::")

    else:
        print("    ::::::::Log dosyası bulunamadı.:::::")
        command = f"platformio run --target upload | tee {log_file_path}"
        process = subprocess.Popen(command, shell=True)
        env.Exit(0)



def process_log_file(source, target, env):
    # Log dosyasını işlemek ve ardından temizlemek için kullanılır
    if os.path.exists(log_file_path):
        with open(log_file_path, 'r') as f:
            log_content = f.read()
            # MAC adresini regex ile çıkarma
            mac_pattern = r"MAC: ([0-9A-Fa-f:]{2}(:[0-9A-Fa-f]{2}){5})"
            match = re.search(mac_pattern, log_content)
            if match:
                mac_address = match.group(1).upper()
                print(f"    ::::MAC Address bulundu")
                project_name = get_project_name()
                save_mac_address(mac_address, project_name)
            else:
                print("    ::::MAC Address log dosyasında bulunamadı.")
        


def save_mac_address(mac, project_name):
    # Yükleme zamanını alıyoruz
    upload_time = datetime.now().strftime("%Y-%m-%d %H:%M:%S")
    # Git versiyonunu alıyoruz
    version_major, version_minor, version_patch, _ = get_git_version_and_date()
    version_string = f"v{version_major}.{version_minor}.{version_patch}"
    
    print(f"    ::::MAC adresini kaydediyorum: {mac} - {upload_time} - {project_name} - {version_string}")

    # Mevcut dosyayı oku
    if os.path.exists(cihazListesi_file_path):
        with open(cihazListesi_file_path, "r") as f:
            lines = f.readlines()
    else:
        lines = []

    # MAC adresini içeren satırları filtrele
    lines = [line for line in lines if not line.startswith(mac)]

    # Yeni satırı oluştur ve listeye ekle
    new_line = f"{mac} , {project_name} , {version_string} , {upload_time}\n"
    lines.append(new_line)

    # Dosyayı güncellenmiş liste ile tekrar yaz
    with open(cihazListesi_file_path, "w") as f:
        f.writelines(lines)




env.AddPreAction("upload", custom_upload_action)
env.AddPostAction("upload", process_log_file)
# env.AddPreAction("buildprog", update_rev_h)
# Script her çalıştırıldığında bu fonksiyonu çalıştırın
update_rev_h(env)
