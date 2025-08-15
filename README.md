# Gelişmiş Binary Analyzer (Advanced Binary Analyzer)

[![C++](https://img.shields.io/badge/C++-17-blue.svg)](https://isocpp.org/)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey.svg)]()

## 📖 Açıklama / Description

**Türkçe:** Bu proje, binary dosyaları analiz etmek için geliştirilmiş gelişmiş bir C++ uygulamasıdır. Dosya türü tespiti, hex dump, string extraction, pattern arama ve detaylı analiz raporları gibi özellikler sunar.

**English:** This project is an advanced C++ application developed for analyzing binary files. It offers features such as file type detection, hex dump, string extraction, pattern searching, and detailed analysis reports.

## ✨ Özellikler / Features

- 🔍 **Dosya Türü Tespiti** - File Type Detection
- 📊 **Hex + ASCII Dump** - Renkli ve adresli görüntüleme
- 🔢 **Çoklu Checksum** - Multiple checksum calculations (Simple, XOR, Fletcher-16)
- 📈 **Byte Frekans Analizi** - Byte frequency analysis with entropy calculation
- 📝 **String Extraction** - Configurable minimum length string extraction
- 🎯 **Pattern Arama** - Hex and string pattern searching
- 📋 **Detaylı Raporlama** - Detailed analysis reports in Turkish
- 🎨 **Modern Kullanıcı Arayüzü** - Modern user interface with menu system

## 🚀 Kurulum / Installation

### Gereksinimler / Requirements
- C++17 uyumlu derleyici (GCC, Clang, MSVC)
- CMake 3.10+ (opsiyonel / optional)

### Derleme / Compilation

#### Windows (MinGW/MSVC)
```bash
# MinGW ile
g++ -std=c++17 -o binary_analyzer.exe app3.cpp

# MSVC ile
cl /std:c++17 app3.cpp /Fe:binary_analyzer.exe
```

#### Linux/macOS
```bash
g++ -std=c++17 -o binary_analyzer app3.cpp
```

#### CMake ile (Cross-platform)
```bash
mkdir build && cd build
cmake ..
make
```

## 📖 Kullanım / Usage

### Temel Kullanım / Basic Usage
```bash
# Windows
binary_analyzer.exe

# Linux/macOS
./binary_analyzer
```

### Program Akışı / Program Flow
1. Analiz edilecek dosya adını girin
2. Otomatik analiz çalıştırılır:
   - Dosya türü tespiti
   - Checksum hesaplamaları
   - Byte frekans analizi
   - String extraction
   - İlk 256 byte hex dump
3. İnteraktif menü ile ek analizler:
   - Pattern arama
   - Belirli aralık hex dump
   - String extraction (farklı minimum uzunluk)
   - Tam hex dump

### Örnek Çıktı / Sample Output
```
=== GELISMIS BINARY ANALYZER ===
Analiz edilecek dosya: example.exe
Dosya yüklendi: 1234567 byte

=== FILE TYPE DETECTION ===
Detected: PE Executable

=== CHECKSUMS ===
Simple Sum: 12345678 (0xBC614E)
XOR Checksum: 42 (0x2A)
Fletcher-16: 12345 (0x3039)

=== BYTE FREQUENCY ANALYSIS ===
En sık gecen 10 byte:
0x00 (.): 12345 kez (1.00%)
0xFF (ÿ): 9876 kez (0.80%)
...
```

## 📁 Proje Yapısı / Project Structure

```
binary-analyzer_cpp/
├── app3.cpp              # Ana kaynak kod / Main source code
├── README.md             # Bu dosya / This file
├── LICENSE               # MIT lisansı / MIT license
├── CMakeLists.txt        # CMake yapılandırması / CMake configuration
├── .gitignore            # Git ignore dosyası / Git ignore file
├── examples/             # Örnek dosyalar / Example files
│   └── sample.bin
└── docs/                 # Dokümantasyon / Documentation
    └── API.md
```

## 🔧 Geliştirme / Development

### Kod Yapısı / Code Structure
- `BinaryAnalyzer` sınıfı ana analiz işlevselliğini sağlar
- Modüler tasarım ile kolay genişletilebilir
- Türkçe ve İngilizce karışık kullanım (geliştirilebilir)

### Katkıda Bulunma / Contributing
1. Fork yapın
2. Feature branch oluşturun (`git checkout -b feature/AmazingFeature`)
3. Değişikliklerinizi commit edin (`git commit -m 'Add some AmazingFeature'`)
4. Branch'inizi push edin (`git push origin feature/AmazingFeature`)
5. Pull Request oluşturun

## 📝 Lisans / License

Bu proje MIT lisansı altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına bakın.

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.


⭐ Bu projeyi beğendiyseniz yıldız vermeyi unutmayın! / Don't forget to star this project if you liked it!
