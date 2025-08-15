#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <cctype>
#include <sstream>
#include <map>
#include <string>
#include <algorithm>
#include <cstring>
#include <memory>
#include <cmath>

class BinaryAnalyzer {
private:
    std::vector<uint8_t> data;
    std::string filename;
    std::ofstream report;

public:
    BinaryAnalyzer(const std::string& fname) : filename(fname) {
        loadFile();
        report.open("analiz_raporu.txt");
        if (report.is_open()) {
            report << "=== BINARY ANALYSIS REPORT ===\n";
            report << "Dosya: " << filename << "\n";
            report << "Boyut: " << data.size() << " byte\n";
            report << "Analız Tarıhı: " << __DATE__ << " " << __TIME__ << "\n\n";
        }
    }

    ~BinaryAnalyzer() {
        if (report.is_open()) {
            report.close();
        }
    }

    // Gelişmiş hex görüntüleme (renkli, adresli)
    void printHexASCII(size_t start = 0, size_t length = 0) {
        if (length == 0) length = data.size();
        const size_t width = 16;
        
        std::cout << "\n=== HEX + ASCII DUMP ===\n";
        std::cout << "Offset    00 01 02 03 04 05 06 07  08 09 0A 0B 0C 0D 0E 0F  ASCII\n";
        std::cout << "--------  --------------------------------  --------------------------------  ----------------\n";
        
        for(size_t i = start; i < start + length && i < data.size(); i += width) {
            // Offset
            std::cout << std::setw(8) << std::setfill('0') << std::hex << std::uppercase << i << "  ";
            
            // Hex bytes (ilk 8)
            for(size_t j = 0; j < 8 && i+j < data.size() && i+j < start + length; ++j) {
                std::cout << std::setw(2) << (int)data[i+j] << " ";
            }
            std::cout << " ";
            
            // Hex bytes (son 8)
            for(size_t j = 8; j < width && i+j < data.size() && i+j < start + length; ++j) {
                std::cout << std::setw(2) << (int)data[i+j] << " ";
            }
            
            // Boşluk doldur
            if(i + width > data.size() || i + width > start + length) {
                size_t missing = width - std::min(width, std::min(data.size() - i, start + length - i));
                for(size_t k = 0; k < missing * 3; ++k) std::cout << " ";
            }
            
            std::cout << "  ";
            
            // ASCII
            for(size_t j = 0; j < width && i+j < data.size() && i+j < start + length; ++j) {
                char c = data[i+j];
                std::cout << (isprint(c) ? c : '.');
            }
            std::cout << std::endl;
        }
    }

    // Çoklu checksum hesaplama
    void calculateChecksums() {
        std::cout << "\n=== CHECKSUMS ===\n";
        
        // Simple checksum
        uint32_t simpleSum = 0;
        for(auto b : data) simpleSum += b;
        std::cout << "Simple Sum: " << simpleSum << " (0x" << std::hex << simpleSum << ")\n";
        
        // XOR checksum
        uint8_t xorSum = 0;
        for(auto b : data) xorSum ^= b;
        std::cout << "XOR Checksum: " << (int)xorSum << " (0x" << std::hex << (int)xorSum << ")\n";
        
        // Fletcher-16
        uint16_t fletcher16 = calculateFletcher16();
        std::cout << "Fletcher-16: " << fletcher16 << " (0x" << std::hex << fletcher16 << ")\n";
        
        if (report.is_open()) {
            report << "CHECKSUMS:\n";
            report << "Simple Sum: " << std::dec << simpleSum << " (0x" << std::hex << simpleSum << ")\n";
            report << "XOR: " << std::dec << (int)xorSum << " (0x" << std::hex << (int)xorSum << ")\n";
            report << "Fletcher-16: " << std::dec << fletcher16 << " (0x" << std::hex << fletcher16 << ")\n\n";
        }
    }

    // Byte frekans analizi
    void analyzeFrequency() {
        std::map<uint8_t, size_t> freq;
        for(auto b : data) freq[b]++;
        
        std::cout << "\n=== BYTE FREQUENCY ANALYSIS ===\n";
        std::cout << "En sık gecen 10 byte:\n";
        
        std::vector<std::pair<uint8_t, size_t>> sortedFreq(freq.begin(), freq.end());
        std::sort(sortedFreq.begin(), sortedFreq.end(), 
                  [](const auto& a, const auto& b) { return a.second > b.second; });
        
        for(size_t i = 0; i < std::min(size_t(10), sortedFreq.size()); ++i) {
            uint8_t byte = sortedFreq[i].first;
            size_t count = sortedFreq[i].second;
            double percent = (double)count / data.size() * 100.0;
            
            std::cout << "0x" << std::hex << std::setw(2) << (int)byte 
                      << " (" << (isprint(byte) ? (char)byte : '.') << "): "
                      << std::dec << count << " kez (" << std::fixed << std::setprecision(2) 
                      << percent << "%)\n";
        }
        
        // Entropy hesaplama
        double entropy = calculateEntropy(freq);
        std::cout << "Shannon Entropy: " << std::fixed << std::setprecision(4) << entropy << " bits\n";
        
        if (report.is_open()) {
            report << "FREQUENCY ANALYSIS:\n";
            report << "Shannon Entropy: " << entropy << " bits\n";
            report << "Top 5 bytes:\n";
            for(size_t i = 0; i < std::min(size_t(5), sortedFreq.size()); ++i) {
                report << "0x" << std::hex << (int)sortedFreq[i].first 
                       << ": " << std::dec << sortedFreq[i].second << " times\n";
            }
            report << "\n";
        }
    }

    // String extraction
    void extractStrings(size_t minLength = 4) {
        std::cout << "\n=== STRING EXTRACTION (min " << minLength << " chars) ===\n";
        
        std::vector<std::pair<size_t, std::string>> strings;
        std::string current;
        size_t startPos = 0;
        
        for(size_t i = 0; i < data.size(); ++i) {
            char c = data[i];
            if(isprint(c) && c != '\t' && c != '\r' && c != '\n') {
                if(current.empty()) startPos = i;
                current += c;
            } else {
                if(current.length() >= minLength) {
                    strings.push_back({startPos, current});
                }
                current.clear();
            }
        }
        
        // Son string'i kontrol et
        if(current.length() >= minLength) {
            strings.push_back({startPos, current});
        }
        
        std::cout << "Bulunan " << strings.size() << " string:\n";
        for(size_t i = 0; i < std::min(size_t(20), strings.size()); ++i) {
            std::cout << "0x" << std::hex << std::setw(8) << strings[i].first 
                      << ": \"" << strings[i].second << "\"\n";
        }
        
        if(strings.size() > 20) {
            std::cout << "... ve " << (strings.size() - 20) << " tane daha\n";
        }
        
        if (report.is_open()) {
            report << "STRING EXTRACTION:\n";
            report << "Found " << strings.size() << " strings (min " << minLength << " chars)\n";
            for(const auto& str : strings) {
                report << "0x" << std::hex << str.first << ": \"" << str.second << "\"\n";
            }
            report << "\n";
        }
    }

    // Gelişmiş pattern arama
    void searchPatterns() {
        int numPatterns;
        std::cout << "\n=== PATTERN SEARCH ===\n";
        std::cout << "Kac pattern arayacaksınız? ";
        std::cin >> numPatterns;
        
        for(int i = 0; i < numPatterns; ++i) {
            std::cout << "\nPattern #" << i+1 << ":\n";
            std::cout << "1. Hex pattern (örn: DEADBEEF)\n";
            std::cout << "2. String pattern\n";
            std::cout << "Secim (1/2): ";
            
            int choice;
            std::cin >> choice;
            
            if(choice == 1) {
                std::string hexInput;
                std::cout << "Hex pattern: ";
                std::cin >> hexInput;
                auto pattern = hexStringToBytes(hexInput);
                searchPattern(pattern, "Hex Pattern #" + std::to_string(i+1));
            } else if(choice == 2) {
                std::string strInput;
                std::cout << "String pattern: ";
                std::cin.ignore();
                std::getline(std::cin, strInput);
                std::vector<uint8_t> pattern(strInput.begin(), strInput.end());
                searchPattern(pattern, "String Pattern #" + std::to_string(i+1));
            }
        }
    }

    // File header/magic number detection
    void detectFileType() {
        std::cout << "\n=== FILE TYPE DETECTION ===\n";
        
        struct MagicNumber {
            std::vector<uint8_t> signature;
            std::string description;
            size_t offset;
        };
        
        std::vector<MagicNumber> magicNumbers = {
            {{0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A}, "PNG Image", 0},
            {{0xFF, 0xD8, 0xFF}, "JPEG Image", 0},
            {{0x47, 0x49, 0x46, 0x38}, "GIF Image", 0},
            {{0x50, 0x4B, 0x03, 0x04}, "ZIP Archive", 0},
            {{0x50, 0x4B, 0x05, 0x06}, "ZIP Archive (empty)", 0},
            {{0x25, 0x50, 0x44, 0x46}, "PDF Document", 0},
            {{0x4D, 0x5A}, "PE Executable", 0},
            {{0x7F, 0x45, 0x4C, 0x46}, "ELF Executable", 0},
            {{0xCA, 0xFE, 0xBA, 0xBE}, "Java Class File", 0},
            {{0x1F, 0x8B, 0x08}, "GZIP Archive", 0}
        };
        
        bool found = false;
        for(const auto& magic : magicNumbers) {
            if(data.size() >= magic.signature.size() + magic.offset) {
                bool match = true;
                for(size_t i = 0; i < magic.signature.size(); ++i) {
                    if(data[magic.offset + i] != magic.signature[i]) {
                        match = false;
                        break;
                    }
                }
                if(match) {
                    std::cout << "Detected: " << magic.description << "\n";
                    if (report.is_open()) {
                        report << "FILE TYPE: " << magic.description << "\n\n";
                    }
                    found = true;
                }
            }
        }
        
        if(!found) {
            std::cout << "Bilinmeyen dosya türü\n";
            if (report.is_open()) {
                report << "FILE TYPE: Unknown\n\n";
            }
        }
    }

    // Hex dump belirli aralık
    void hexDumpRange() {
        size_t start, length;
        std::cout << "\n=== HEX DUMP RANGE ===\n";
        std::cout << "Baslangic offset (hex): 0x";
        std::cin >> std::hex >> start;
        std::cout << "Uzunluk (dec): ";
        std::cin >> std::dec >> length;
        
        if(start < data.size()) {
            printHexASCII(start, length);
        } else {
            std::cout << "Gecersiz offset!\n";
        }
    }

    void runFullAnalysis() {
        if(data.empty()) {
            std::cout << "Dosya bos veya yüklenemedi!\n";
            return;
        }
        
        detectFileType();
        calculateChecksums();
        analyzeFrequency();
        extractStrings();
        
        // İlk 256 byte'ı göster
        std::cout << "\n=== İLK 256 BYTE ===";
        printHexASCII(0, 256);
        
        // Menü
        int choice;
        do {
            std::cout << "\n=== MENU ===\n";
            std::cout << "1. Pattern ara\n";
            std::cout << "2. Belirli aralik hex dump\n";
            std::cout << "3. String extraction (farkli min uzunluk)\n";
            std::cout << "4. Tam hex dump\n";
            std::cout << "5. Cikis\n";
            std::cout << "Secim: ";
            std::cin >> choice;
            
            switch(choice) {
                case 1: searchPatterns(); break;
                case 2: hexDumpRange(); break;
                case 3: {
                    size_t minLen;
                    std::cout << "Minimum string uzunlugu: ";
                    std::cin >> minLen;
                    extractStrings(minLen);
                    break;
                }
                case 4: printHexASCII(); break;
                case 5: std::cout << "Analiz tamamlandi.\n"; break;
                default: std::cout << "Gecersiz secim!\n";
            }
        } while(choice != 5);
        
        std::cout << "\nAnaliz raporu 'analiz_raporu.txt' dosyasına kaydedildi.\n";
    }

private:
    void loadFile() {
        std::ifstream file(filename, std::ios::binary);
        if(!file) {
            std::cerr << "Dosya acilamadi: " << filename << "\n";
            return;
        }
        
        data = std::vector<uint8_t>((std::istreambuf_iterator<char>(file)),
                                   std::istreambuf_iterator<char>());
        file.close();
        
        std::cout << "Dosya yüklendi: " << data.size() << " byte\n";
    }

    uint16_t calculateFletcher16() {
        uint16_t sum1 = 0, sum2 = 0;
        for(auto b : data) {
            sum1 = (sum1 + b) % 255;
            sum2 = (sum2 + sum1) % 255;
        }
        return (sum2 << 8) | sum1;
    }

    double calculateEntropy(const std::map<uint8_t, size_t>& freq) {
        double entropy = 0.0;
        size_t total = data.size();
        
        for(const auto& pair : freq) {
            double p = (double)pair.second / total;
            if(p > 0) {
                // log2(p) = log(p) / log(2) - bazı derleyicilerde log2 mevcut değil
                entropy -= p * (std::log(p) / std::log(2.0));
            }
        }
        return entropy;
    }

    std::vector<uint8_t> hexStringToBytes(const std::string& hex) {
        std::vector<uint8_t> bytes;
        std::string cleanHex = hex;
        
        // Remove spaces and make uppercase
        cleanHex.erase(std::remove_if(cleanHex.begin(), cleanHex.end(), isspace), cleanHex.end());
        std::transform(cleanHex.begin(), cleanHex.end(), cleanHex.begin(), ::toupper);
        
        for(size_t i = 0; i < cleanHex.length(); i += 2) {
            if(i + 1 < cleanHex.length()) {
                std::string byteStr = cleanHex.substr(i, 2);
                try {
                    uint8_t byte = (uint8_t)std::stoi(byteStr, nullptr, 16);
                    bytes.push_back(byte);
                } catch(const std::exception& e) {
                    std::cerr << "Geçersiz hex string: " << byteStr << std::endl;
                }
            }
        }
        return bytes;
    }

    void searchPattern(const std::vector<uint8_t>& pattern, const std::string& name) {
        std::vector<size_t> positions;
        
        for(size_t i = 0; i <= data.size() - pattern.size(); ++i) {
            bool match = true;
            for(size_t j = 0; j < pattern.size(); ++j) {
                if(data[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if(match) {
                positions.push_back(i);
            }
        }
        
        if(positions.empty()) {
            std::cout << name << " bulunamadİ.\n";
            if (report.is_open()) {
                report << name << ": Not found\n";
            }
        } else {
            std::cout << name << " " << positions.size() << " kez bulundu:\n";
            for(size_t pos : positions) {
                std::cout << "  0x" << std::hex << std::setw(8) << pos << std::endl;
            }
            
            if (report.is_open()) {
                report << name << ": Found " << positions.size() << " times\n";
                for(size_t pos : positions) {
                    report << "  0x" << std::hex << pos << "\n";
                }
            }
        }
    }
};

int main() {
    std::string filename;
    std::cout << "=== GELISMIS BINARY ANALYZER ===\n";
    std::cout << "Analiz edilecek dosya: ";
    std::cin >> filename;
    
    BinaryAnalyzer analyzer(filename);
    analyzer.runFullAnalysis();
    
    return 0;
}