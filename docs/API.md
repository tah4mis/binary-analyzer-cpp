# Binary Analyzer API Documentation

## Overview

The `BinaryAnalyzer` class provides comprehensive binary file analysis capabilities including file type detection, checksum calculations, frequency analysis, string extraction, and pattern searching.

## Class: BinaryAnalyzer

### Constructor

```cpp
BinaryAnalyzer(const std::string& fname)
```

**Parameters:**
- `fname`: Path to the binary file to analyze

**Description:** Initializes the analyzer, loads the specified file, and opens a report file (`analiz_raporu.txt`).

### Public Methods

#### 1. printHexASCII()

```cpp
void printHexASCII(size_t start = 0, size_t length = 0)
```

**Parameters:**
- `start`: Starting offset in bytes (default: 0)
- `length`: Number of bytes to display (default: 0 = entire file)

**Description:** Displays a formatted hex dump with ASCII representation. Shows offset, hex values, and printable ASCII characters.

**Example:**
```cpp
analyzer.printHexASCII(0, 256);  // Show first 256 bytes
analyzer.printHexASCII();         // Show entire file
```

#### 2. calculateChecksums()

```cpp
void calculateChecksums()
```

**Description:** Calculates and displays multiple checksum types:
- Simple sum (32-bit)
- XOR checksum (8-bit)
- Fletcher-16 checksum (16-bit)

**Output:** Displays checksums in both decimal and hexadecimal format.

#### 3. analyzeFrequency()

```cpp
void analyzeFrequency()
```

**Description:** Performs byte frequency analysis including:
- Top 10 most frequent bytes
- Percentage distribution
- Shannon entropy calculation

**Output:** Frequency statistics and entropy value in bits.

#### 4. extractStrings()

```cpp
void extractStrings(size_t minLength = 4)
```

**Parameters:**
- `minLength`: Minimum string length to extract (default: 4)

**Description:** Extracts printable strings from the binary data, filtering out control characters.

**Output:** List of strings with their offset addresses.

#### 5. searchPatterns()

```cpp
void searchPatterns()
```

**Description:** Interactive pattern searching that supports:
- Hex pattern input (e.g., "DEADBEEF")
- String pattern input
- Multiple pattern searches in one session

**Usage:** User is prompted to choose pattern type and input the pattern to search for.

#### 6. detectFileType()

```cpp
void detectFileType()
```

**Description:** Attempts to identify the file type by checking magic numbers/signatures for common formats:
- PNG, JPEG, GIF images
- ZIP archives
- PDF documents
- PE/ELF executables
- Java class files
- GZIP archives

**Output:** Detected file type or "Unknown" if no match is found.

#### 7. hexDumpRange()

```cpp
void hexDumpRange()
```

**Description:** Interactive hex dump for a specific range. Prompts user for:
- Starting offset (hexadecimal)
- Length (decimal)

**Output:** Hex dump of the specified range.

#### 8. runFullAnalysis()

```cpp
void runFullAnalysis()
```

**Description:** Main analysis workflow that:
1. Performs automatic analysis (file type, checksums, frequency, strings)
2. Shows first 256 bytes
3. Presents interactive menu for additional analysis

**Menu Options:**
1. Pattern search
2. Range hex dump
3. String extraction with custom length
4. Full hex dump
5. Exit

### Private Methods

#### loadFile()

```cpp
void loadFile()
```

**Description:** Loads the binary file into memory as a vector of bytes.

#### calculateFletcher16()

```cpp
uint16_t calculateFletcher16()
```

**Returns:** 16-bit Fletcher checksum value.

**Description:** Implements the Fletcher-16 checksum algorithm.

#### calculateEntropy()

```cpp
double calculateEntropy(const std::map<uint8_t, size_t>& freq)
```

**Parameters:**
- `freq`: Map of byte frequencies

**Returns:** Shannon entropy value in bits.

**Description:** Calculates information entropy based on byte distribution.

#### hexStringToBytes()

```cpp
std::vector<uint8_t> hexStringToBytes(const std::string& hex)
```

**Parameters:**
- `hex`: Hexadecimal string (e.g., "DEADBEEF")

**Returns:** Vector of bytes.

**Description:** Converts hexadecimal string to byte vector.

#### searchPattern()

```cpp
void searchPattern(const std::vector<uint8_t>& pattern, const std::string& name)
```

**Parameters:**
- `pattern`: Byte pattern to search for
- `name`: Name/description of the pattern

**Description:** Searches for the specified pattern in the binary data and reports all occurrences.

## Usage Examples

### Basic Analysis

```cpp
#include "app3.cpp"  // Include the source file

int main() {
    BinaryAnalyzer analyzer("example.exe");
    analyzer.runFullAnalysis();
    return 0;
}
```

### Custom Analysis

```cpp
BinaryAnalyzer analyzer("file.bin");

// Show first 512 bytes
analyzer.printHexASCII(0, 512);

// Extract strings with minimum length 8
analyzer.extractStrings(8);

// Calculate checksums
analyzer.calculateChecksums();
```

## Error Handling

The analyzer handles common errors gracefully:
- File not found: Displays error message and continues
- Invalid hex input: Reports parsing errors
- Invalid offsets: Validates range before processing

## Report Generation

All analysis results are automatically saved to `analiz_raporu.txt` in Turkish, providing a permanent record of the analysis session.

## Performance Notes

- File loading is done once during construction
- Large files (>100MB) may consume significant memory
- Pattern searching uses linear search algorithm (O(n*m) complexity)
- String extraction processes the entire file sequentially
