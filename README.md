# GoatTool 
A Unix-like File System Exploration Utility

## Description
GoatTool is a command-line utility designed to aid users in exploring and understanding the file system of their operating system. It offers various operations that can be accessed by passing specific switches as command-line arguments.


## Build Instructions
Run the following command to make the build script executable:
    
    chmod +x build.sh

Then run the following command to build GoatTool:
    
    ./build.sh    


## Usage
`./goattool [switch] [options]`


## Switches
- `-p <file_paths>`: Print the contents of one or more files.
- `-g <file_path> <search_string>`: Search a file for a particular string and print matching lines.
- `-s <file_paths>`: Print the size of one or more files.
- `-m <file_paths> <output_path>`: Merge multiple files into a single file.
- `-c <input_file_path> <output_file_path>`: Compress a text file into the .goat format.
- `-d <input_file_path> <output_file_path>`: Decompress a .goat file to retrieve the original text.
- `-q <file_paths>`: Print the permissions of one or more files as an integer (000 to 777).
- `-n <source_path> <destination_path> <new_name>`: Move a file to a new location with a new name.
- `-h`: Print this help page.


## Examples
- Print the contents of a file:

`./goattool -p example.txt`

- Search for a string in a file and print matching lines:

`./goattool -g example.txt "search_string"`

- Print the size of a file:

`./goattool -s example.txt`

- Merge multiple files into a new file:

`./goattool -m file1.txt file2.txt merged.txt`

- Compress a text file into .goat format:

`./goattool -c input.txt compressed.goat`

- Decompress a .goat file:

`./goattool -d compressed.goat decompressed.txt`

- Print file permissions:

`./goattool -q file.txt`

- Move a file to a new location with a new name:

`./goattool -n oldfile.txt new_directory/renamedfile.txt`

- Display this help page:

`./goattool -h`


## More Information
For more details and examples, please refer to the documentation provided in the project's [design document](https://docs.google.com/document/d/1I0iIutgpA8jFL-g_fbloi5UC2SqTuxGSyACfpIZnkcU/edit?usp=sharing).
