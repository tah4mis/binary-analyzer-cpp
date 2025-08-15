# Examples Directory

This directory contains sample files for testing the Binary Analyzer.

## Sample Files

### sample.bin
A simple binary file containing various byte patterns for testing:
- ASCII strings
- Binary data
- Different byte frequencies
- Known patterns for testing search functionality

## Usage

1. **Copy a sample file to your working directory:**
   ```bash
   cp examples/sample.bin ./test_file.bin
   ```

2. **Run the analyzer:**
   ```bash
   ./binary_analyzer
   # Enter: test_file.bin
   ```

3. **Test different features:**
   - File type detection
   - Pattern searching (try "HELLO" or "0x12345678")
   - String extraction
   - Frequency analysis

## Creating Your Own Test Files

You can create custom test files using various methods:

### Using Python
```python
# Create a test file with known patterns
with open('test.bin', 'wb') as f:
    # ASCII string
    f.write(b'HELLO WORLD')
    # Some binary data
    f.write(bytes([0x00, 0x01, 0x02, 0x03]))
    # Another string
    f.write(b'BINARY ANALYZER TEST')
    # Random data
    f.write(bytes([0xFF, 0xFE, 0xFD, 0xFC]))
```

### Using dd (Linux/macOS)
```bash
# Create a 1KB file with random data
dd if=/dev/urandom of=random.bin bs=1024 count=1

# Create a file with specific pattern
echo -n "TEST PATTERN" > pattern.bin
```

### Using PowerShell (Windows)
```powershell
# Create a test file
[System.Text.Encoding]::ASCII.GetBytes("WINDOWS TEST FILE") | Set-Content -Path "test.bin" -Encoding Byte
```

## Expected Results

When analyzing the sample files, you should see:
- String extraction results
- Frequency analysis showing byte distribution
- Checksum calculations
- Pattern search results (if patterns exist)

## Troubleshooting

- **File not found errors:** Ensure the file path is correct
- **Permission errors:** Check file read permissions
- **Large files:** The analyzer loads entire files into memory
- **Binary vs text:** The analyzer works with any binary file type
