from typing import Any
from SCons.Script import Import  # type: ignore
Import("env")
env: Any  # Type hint ekle

import os
import shutil
from datetime import datetime

def log_conversion(message, status='info'):
    """Dönüştürme işlemlerini logla"""
    symbols = {
        'success': '✓',
        'error': '✗',
        'info': 'ℹ',
        'skip': '⤳'
    }
    timestamp = datetime.now().strftime('%H:%M:%S')
    print(f"[{timestamp}] {symbols.get(status, 'ℹ')} {message}")

def clean_comments(content):
    """HTML dosyasındaki gereksiz yorumları ve kapanış tag'lerini temizler."""
    
    # Satır satır işle
    lines = content.split('\n')
    cleaned_lines = []
    in_style = False
    in_script = False
    
    for line in lines:
        stripped = line.strip()
        
        # Son kapanış tag'lerini atla
        if stripped in ['</script>', '</body>', '</html>']:
            continue
            
        # Style ve script bloklarını takip et
        if '<style>' in line: in_style = True
        if '</style>' in line: in_style = False
        if '<script>' in line: in_script = True
        if '</script>' in line and not stripped == '</script>': in_script = False
        
        # Boş satırları atla
        if not stripped:
            continue
            
        # HTML yorumlarını temizle
        if '<!--' in line and '-->' in line:
            continue
            
        # CSS yorumlarını temizle (style bloğu içinde)
        if in_style and '/*' in line and '*/' in line:
            # Sadece yorum varsa satırı atla
            if stripped.startswith('/*') and stripped.endswith('*/'):
                continue
            # Yoksa yorumu temizle
            line = line.split('/*')[0].rstrip()
            
        # JavaScript yorumlarını temizle (script bloğu içinde)
        if in_script:
            # Tek satırlık yorumları kontrol et
            if '//' in line:
                code = line.split('//')[0].rstrip()
                # Eğer koddan önce yorum varsa satırı atla
                if not code:
                    continue
                line = code
                
        cleaned_lines.append(line)
    
    return '\n'.join(cleaned_lines)

def convert_html_to_header(source, target, env):
    # Kaynak ve hedef dizinler
    src_dir = 'captive'
    dst_dir = 'src/web'
    
    try:
        # Hedef dizin yoksa oluştur
        if not os.path.exists(dst_dir):
            os.makedirs(dst_dir)
            log_conversion(f"Created directory: {dst_dir}")
        
        # Mevcut header dosyalarını tara
        existing_headers = {f for f in os.listdir(dst_dir) if f.endswith('HTML.h')}
        converted_files = set()
        
        # HTML dosyalarını işle
        for filename in os.listdir(src_dir):
            if filename.endswith('.html'):
                try:
                    base_name = os.path.splitext(filename)[0]
                    header_name = f"{base_name}HTML.h"
                    const_name = f"{base_name.capitalize()}_Html"
                    
                    src_path = os.path.join(src_dir, filename)
                    dst_path = os.path.join(dst_dir, header_name)
                    
                    # Zaman damgası kontrolü
                    if os.path.exists(dst_path):
                        src_time = os.path.getmtime(src_path)
                        dst_time = os.path.getmtime(dst_path)
                        if src_time <= dst_time:
                            log_conversion(f"Skipping {filename} (not modified)", 'skip')
                            converted_files.add(header_name)
                            continue
                    
                    # HTML dosyasını oku ve dönüştür
                    with open(src_path, 'r', encoding='utf-8') as f:
                        content = f.read()
                    
                    # Yorumları temizle
                    content = clean_comments(content)
                    
                    # Boş satırları temizle
                    content = '\n'.join(line for line in content.split('\n') if line.strip())
                    
                    # Header içeriğini oluştur
                    header_content = f'''#include "header.h"

const char {const_name}[] PROGMEM = R"rawliteral(
{content}
)rawliteral";'''
                    
                    # Header dosyasını yaz
                    with open(dst_path, 'w', encoding='utf-8') as f:
                        f.write(header_content)
                    
                    converted_files.add(header_name)
                    log_conversion(f"Converted: {filename} -> {header_name}", 'success')
                    
                except Exception as e:
                    log_conversion(f"Error converting {filename}: {str(e)}", 'error')
        
        # Silinen HTML'lere ait header dosyalarını temizle
        for header in existing_headers - converted_files:
            try:
                os.remove(os.path.join(dst_dir, header))
                log_conversion(f"Removed unused header: {header}", 'info')
            except Exception as e:
                log_conversion(f"Error removing {header}: {str(e)}", 'error')
                
    except Exception as e:
        log_conversion(f"Global error: {str(e)}", 'error')
        raise e

# Pre-build hook'u ekle
env.AddPreAction("buildprog", convert_html_to_header) 