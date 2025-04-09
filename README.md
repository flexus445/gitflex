# GitFlex

A lightweight Git implementation for learning purposes. This project implements basic Git plumbing commands to understand how Git works under the hood.

## Commands

GitFlex currently implements the following basic Git plumbing commands:

1. **init**
   - Initializes a new GitFlex repository
   - Creates the `.gitflex` directory structure
   - Sets up the initial HEAD reference

2. **hash-object**
   - Creates a blob object from a file
   - Computes the SHA-1 hash of the file content
   - Stores the compressed object in the `.gitflex/objects` directory

3. **cat-file**
   - Displays the contents of a Git object
   - Decompresses and shows the stored content
   - Shows both the object type and content

4. **write-tree**
   - Creates a tree object from the current directory
   - Recursively processes subdirectories
   - Stores file modes and hashes in the tree structure

## Testing

The project includes a test suite using CUnit to verify the functionality of each command:

- **test_init**: Verifies repository initialization
  - Checks directory structure creation
  - Validates HEAD file content

- **test_hash_object**: Tests blob object creation
  - Verifies object storage
  - Checks hash computation

- **test_cat_file**: Tests object content retrieval
  - Verifies content decompression
  - Checks object type and content display

## Project Structure

```
gitflex/
├── include/           # Header files
│   └── gitflex.h     # Public interface declarations
├── src/              # Implementation files
│   └── gitflex_impl.c # Core Git implementation
├── tests/            # Test files
│   ├── test_gitflex.c  # Test cases
│   └── test_runner.c   # Test runner
├── main.c           # Main program entry point
└── CMakeLists.txt   # Build configuration
```

## Building and Testing

1. Install dependencies:
   ```bash
   brew install cunit openssl
   ```

2. Build the project:
   ```bash
   mkdir -p build
   cd build
   cmake ..
   make
   ```

3. Run tests:
   ```bash
   ctest
   ```

## Usage

```bash
# Initialize a new repository
./gitflex init

# Create a blob object from a file
./gitflex hash-object <file>

# Display object contents
./gitflex cat-file <hash>

# Create a tree object
./gitflex write-tree
```