import subprocess
import tempfile
import unittest
import os
import stat
import shutil

# Base case tests (shared functions)
class BaseTestGoatTool(unittest.TestCase):
    def create_temp_file(self, content="", extension=".txt"):
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=extension, mode='w')
        temp_file.write(content)
        temp_file.close()
        return temp_file.name
    
    def create_temp_directory(self, name="new_directory"):
        temp_dir = os.path.join(tempfile.gettempdir(), name)
        os.makedirs(temp_dir, exist_ok=True)
        return temp_dir
    
    def set_file_permissions(self, file_path, permissions):
        os.chmod(file_path, permissions)

# Tests for -p switch
class TestGoatToolPrintFunction(BaseTestGoatTool):

    def test_print_existing_file(self):
        temp_file_path = self.create_temp_file("Content of the existing file.")
        result = subprocess.run(['./GoatTool', '-p', temp_file_path], capture_output=True, text=True)
        os.remove(temp_file_path)
        self.assertEqual(result.stdout, "Content of the existing file.\n")

    def test_print_nonexistent_file(self):
        result = subprocess.run(['./GoatTool', '-p', 'nonexistent_file.txt'], capture_output=True, text=True)
        print("STDOUT:", result.stdout)
        print("STDERR:", result.stderr)
        # Adjust the assertion based on the actual output

    def test_print_no_file_specified(self):
        result = subprocess.run(['./GoatTool', '-p'], capture_output=True, text=True)
        print("STDOUT:", result.stdout)
        print("STDERR:", result.stderr)
        # Adjust the assertion based on the actual output

    def test_print_multiple_files(self):
        temp_file1_path = self.create_temp_file("Content of the first file.")
        temp_file2_path = self.create_temp_file("Content of the second file.")
        result = subprocess.run(['./GoatTool', '-p', temp_file1_path, temp_file2_path], capture_output=True, text=True)
        os.remove(temp_file1_path)
        os.remove(temp_file2_path)
        expected_output = "Content of the first file.\nContent of the second file.\n"
        self.assertEqual(result.stdout, expected_output)

    def test_print_unsupported_format(self):
        temp_file_path = self.create_temp_file(extension=".doc")
        result = subprocess.run(['./GoatTool', '-p', temp_file_path], capture_output=True, text=True)
        print("STDOUT:", result.stdout)
        print("STDERR:", result.stderr)
        # Adjust the assertion based on the actual output

# Tests for -g switch
class TestGoatToolSearchString(BaseTestGoatTool):

    def test_search_string_in_existing_file(self):
        temp_file_path = self.create_temp_file("This is a test file with search_string.")
        result = subprocess.run(['./GoatTool', '-g', temp_file_path, "search_string"], capture_output=True, text=True)
        os.remove(temp_file_path)
        self.assertIn("search_string", result.stdout)

    def test_search_string_in_nonexistent_file(self):
        result = subprocess.run(['./GoatTool', '-g', 'nonexistent_file.txt', "search_string"], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: File does not exist.\n")

    def test_search_nonexistent_string_in_file(self):
        temp_file_path = self.create_temp_file("This is a test file.")
        result = subprocess.run(['./GoatTool', '-g', temp_file_path, "nonexistent_string"], capture_output=True, text=True)
        os.remove(temp_file_path)
        self.assertEqual(result.stdout, "String not found.\n")

    def test_search_without_file_or_string(self):
        result = subprocess.run(['./GoatTool', '-g'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: File or search string not specified.\n")

    def test_search_without_string(self):
        temp_file_path = self.create_temp_file("Some content.")
        result = subprocess.run(['./GoatTool', '-g', temp_file_path], capture_output=True, text=True)
        os.remove(temp_file_path)
        self.assertEqual(result.stdout, "ERROR: Search string not specified.\n")

# Tests for -s switch
class TestGoatToolFileSize(BaseTestGoatTool):

    def test_print_size_existing_file(self):
        content = "This is a file of size 30 bytes."  # Example content
        temp_file_path = self.create_temp_file(content)
        actual_size = str(len(content))  # Calculate the actual size of the content
        result = subprocess.run(['./GoatTool', '-s', temp_file_path], capture_output=True, text=True)
        os.remove(temp_file_path)
        expected_output = f"Size of {os.path.basename(temp_file_path)}: {actual_size} bytes\n"
        self.assertEqual(result.stdout, expected_output)

    def test_print_size_nonexistent_file(self):
        result = subprocess.run(['./GoatTool', '-s', 'nonexistent_file.txt'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: File does not exist.\n")

    def test_print_size_no_file_specified(self):
        result = subprocess.run(['./GoatTool', '-s'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: No file specified.\n")

    def test_print_size_multiple_files(self):
        content1 = "Content for first file"
        content2 = "Content for second file"
        temp_file1_path = self.create_temp_file(content1)
        temp_file2_path = self.create_temp_file(content2)
        size1 = str(len(content1))
        size2 = str(len(content2))
        result = subprocess.run(['./GoatTool', '-s', temp_file1_path, temp_file2_path], capture_output=True, text=True)
        os.remove(temp_file1_path)
        os.remove(temp_file2_path)
        expected_output = f"Size of {os.path.basename(temp_file1_path)}: {size1} bytes\nSize of {os.path.basename(temp_file2_path)}: {size2} bytes\n"
        self.assertEqual(result.stdout, expected_output)

    def test_print_size_unsupported_format(self):
        temp_file_path = self.create_temp_file("Some content", ".doc")
        result = subprocess.run(['./GoatTool', '-s', temp_file_path], capture_output=True, text=True)
        os.remove(temp_file_path)
        self.assertEqual(result.stdout, "ERROR: Incorrect file format.\n")


# Tests for -m switch
class TestGoatToolMergeFiles(BaseTestGoatTool):
    
    def test_merge_files_successfully(self):
        file1_path = self.create_temp_file("Content of file 1")
        file2_path = self.create_temp_file("Content of file 2")
        merged_file_path = tempfile.NamedTemporaryFile(delete=False, suffix=".txt").name
        result = subprocess.run(['./GoatTool', '-m', file1_path, file2_path, merged_file_path], capture_output=True, text=True)
        os.remove(file1_path)
        os.remove(file2_path)
        os.remove(merged_file_path)
        self.assertEqual(result.stdout, "Files merged successfully.\n")

    def test_merge_with_nonexistent_file(self):
        file1_path = self.create_temp_file("Content of file 1")
        merged_file_path = tempfile.NamedTemporaryFile(delete=False, suffix=".txt").name
        result = subprocess.run(['./GoatTool', '-m', 'nonexistent_file.txt', file1_path, merged_file_path], capture_output=True, text=True)
        os.remove(file1_path)
        os.remove(merged_file_path)
        self.assertIn("ERROR: nonexistent_file.txt does not exist.", result.stdout)

    def test_merge_no_files_specified(self):
        result = subprocess.run(['./GoatTool', '-m'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: Files to merge not specified.\n")

    def test_merge_no_destination_file_specified(self):
        file1_path = self.create_temp_file("Content")
        result = subprocess.run(['./GoatTool', '-m', file1_path], capture_output=True, text=True)
        os.remove(file1_path)
        self.assertEqual(result.stdout, "ERROR: Files to merge not specified.\n")

    def test_merge_to_nonexistent_directory(self):
        file1_path = self.create_temp_file("Content")
        result = subprocess.run(['./GoatTool', '-m', file1_path, 'nonexistent_directory/merged.txt'], capture_output=True, text=True)
        os.remove(file1_path)
        self.assertIn("ERROR: Destination file not specified.\n", result.stdout)

# Tests for -c switch
class TestGoatToolCompressFunction(BaseTestGoatTool):

    def test_compress_valid_file(self):
        original_content = "hhhhTTTTT mmmmmmmsss"
        expected_compressed_content = "h4T5 m7s3"
        input_file_path = self.create_temp_file(original_content, ".txt")
        output_file_path = tempfile.NamedTemporaryFile(delete=False, suffix=".goat").name
        result = subprocess.run(['./GoatTool', '-c', input_file_path, output_file_path], capture_output=True, text=True)
        
        with open(output_file_path, 'r') as file:
            compressed_content = file.read()

        os.remove(input_file_path)
        os.remove(output_file_path)
        self.assertEqual(compressed_content, expected_compressed_content)
        self.assertEqual(result.stdout, "File compressed successfully.\n")

    def test_compress_nonexistent_file(self):
        output_file_path = tempfile.NamedTemporaryFile(delete=False, suffix=".goat").name
        result = subprocess.run(['./GoatTool', '-c', 'nonexistent_file.txt', output_file_path], capture_output=True, text=True)
        os.remove(output_file_path)
        self.assertEqual(result.stdout, "Error: Unable to read file\n")

    def test_compress_no_input_file(self):
        result = subprocess.run(['./GoatTool', '-c'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "Error: Incorrect number of arguments for compression operation\n")

    def test_compress_no_output_file(self):
        input_file_path = self.create_temp_file("Some content", ".txt")
        result = subprocess.run(['./GoatTool', '-c', input_file_path], capture_output=True, text=True)
        os.remove(input_file_path)
        self.assertEqual(result.stdout, "Error: Incorrect number of arguments for compression operation\n")

    def test_compress_unsupported_format(self):
        input_file_path = self.create_temp_file("Content", ".doc")
        output_file_path = tempfile.NamedTemporaryFile(delete=False, suffix=".goat").name
        result = subprocess.run(['./GoatTool', '-c', input_file_path, output_file_path], capture_output=True, text=True)
        os.remove(input_file_path)
        os.remove(output_file_path)
        self.assertEqual(result.stdout, "Error: Unable to read file\n")

# Tests for -d switch
class TestGoatToolDecompressFunction(BaseTestGoatTool):

    def test_decompress_valid_file(self):
        compressed_content = "h4T5 m7s3"
        expected_decompressed_content = "hhhhTTTTT mmmmmmmsss"
        compressed_file_path = self.create_temp_file(compressed_content, ".goat")
        new_file_path = compressed_file_path.replace('.goat', '.txt')

        subprocess.run(['./GoatTool', '-d', compressed_file_path], capture_output=True, text=True)

        with open(new_file_path, 'r') as file:
            decompressed_content = file.read()

        self.assertEqual(decompressed_content, expected_decompressed_content)
        
        # Clean up
        os.remove(new_file_path)


    def test_decompress_nonexistent_file(self):
        result = subprocess.run(['./GoatTool', '-d', 'nonexistent_file.goat', 'normal.txt'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "Error: Incorrect number of arguments for decompression operation\n")

    def test_decompress_no_input_file(self):
        result = subprocess.run(['./GoatTool', '-d'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "Error: Incorrect number of arguments for decompression operation\n")

    def test_decompress_no_output_file(self):
        compressed_file_path = self.create_temp_file("Simulated compressed content", ".goat")
        new_file_path = compressed_file_path.replace('.goat', '.txt')  # Expecting a .txt file after decompression

        result = subprocess.run(['./GoatTool', '-d', compressed_file_path], capture_output=True, text=True)
        
        # Check for the existence of the new .txt file instead of trying to remove the .goat file
        self.assertTrue(os.path.exists(new_file_path), "Decompressed .txt file not found")

        # Clean up: remove the new .txt file if it exists
        if os.path.exists(new_file_path):
            os.remove(new_file_path)

    def test_decompress_unsupported_format(self):
        compressed_file_path = self.create_temp_file("Simulated content", ".doc")
        output_file_path = tempfile.NamedTemporaryFile(delete=False, suffix=".txt").name
        result = subprocess.run(['./GoatTool', '-d', compressed_file_path, output_file_path], capture_output=True, text=True)
        os.remove(compressed_file_path)
        os.remove(output_file_path)
        self.assertEqual(result.stdout, "Error: Incorrect number of arguments for decompression operation\n")



# Tests for -q switch
class TestGoatToolPrintPermissions(BaseTestGoatTool):

    def test_print_permissions_existing_file(self):
        temp_file_path = self.create_temp_file("Content")
        self.set_file_permissions(temp_file_path, 0o755)  # Set permissions to 755
        result = subprocess.run(['./GoatTool', '-q', temp_file_path], capture_output=True, text=True)
        os.remove(temp_file_path)
        expected_output = f"Permissions of {os.path.basename(temp_file_path)}: 755\n"
        self.assertEqual(result.stdout, expected_output)

    def test_print_permissions_nonexistent_file(self):
        result = subprocess.run(['./GoatTool', '-q', 'nonexistent_file.txt'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: File does not exist.\n")

    def test_print_permissions_no_file_specified(self):
        result = subprocess.run(['./GoatTool', '-q'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: No file specified.\n")

    def test_print_permissions_multiple_files(self):
        temp_file1_path = self.create_temp_file("Content")
        temp_file2_path = self.create_temp_file("Content")
        self.set_file_permissions(temp_file1_path, 0o755)  # 755 permissions
        self.set_file_permissions(temp_file2_path, 0o644)  # 644 permissions
        result = subprocess.run(['./GoatTool', '-q', temp_file1_path, temp_file2_path], capture_output=True, text=True)
        os.remove(temp_file1_path)
        os.remove(temp_file2_path)
        expected_output = f"Permissions of {os.path.basename(temp_file1_path)}: 755\nPermissions of {os.path.basename(temp_file2_path)}: 644\n"
        self.assertEqual(result.stdout, expected_output)

    def test_print_permissions_directory(self):
        temp_dir = tempfile.TemporaryDirectory()
        result = subprocess.run(['./GoatTool', '-q', temp_dir.name], capture_output=True, text=True)
        temp_dir.cleanup()
        self.assertEqual(result.stdout, "ERROR: Path is a directory, not a file.\n")

# TODO: Tests for -n switch
class TestGoatToolMoveAndRename(BaseTestGoatTool):
    def test_move_and_rename_file_successfully(self):
        old_file_path = self.create_temp_file("Content")
        new_directory = self.create_temp_directory()
        new_file_name = "newfile.txt"
        new_file_path = os.path.join(new_directory, new_file_name)
        result = subprocess.run(['./GoatTool', '-n', old_file_path, new_directory, new_file_name], capture_output=True, text=True)
        
        file_moved = not os.path.exists(old_file_path) and os.path.exists(new_file_path)
        shutil.rmtree(new_directory)
        self.assertTrue(file_moved, "File was not moved and renamed successfully")


    def test_move_nonexistent_file(self):
        new_directory = self.create_temp_directory()
        result = subprocess.run(['./GoatTool', '-n', 'nonexistent_file.txt', new_directory, 'newfile.txt'], capture_output=True, text=True)
        shutil.rmtree(new_directory)
        self.assertEqual(result.stdout, "ERROR: File does not exist.\n")

    def test_move_no_source_file_specified(self):
        result = subprocess.run(['./GoatTool', '-n'], capture_output=True, text=True)
        self.assertEqual(result.stdout, "ERROR: Source file not specified.\n")

    def test_move_no_destination_specified(self):
        old_file_path = self.create_temp_file("Content")
        result = subprocess.run(['./GoatTool', '-n', old_file_path], capture_output=True, text=True)
        os.remove(old_file_path)
        self.assertEqual(result.stdout, "ERROR: Destination path or new name not specified.\n")

    def test_move_to_nonexistent_directory(self):
        old_file_path = self.create_temp_file("Content")
        result = subprocess.run(['./GoatTool', '-n', old_file_path, 'nonexistent_directory/', 'newfile.txt'], capture_output=True, text=True)
        os.remove(old_file_path)
        self.assertEqual(result.stdout, "ERROR: Destination directory does not exist.\n")

# Tests for -h switch
class TestGoatToolHelpPage(BaseTestGoatTool):
    def test_display_help_page(self):
        result = subprocess.run(['./GoatTool', '-h'], capture_output=True, text=True)
        expected_help_page = (
            "GoatTool Help Page:\n"
            " -p <file_paths>       Print the contents of one or more files.\n"
            " -g <file_path> <search_string> Search a file for a particular string and print matching lines.\n"
            " -s <file_paths>       Print the size of one or more files.\n"
            " -m <file_paths> <output_path> Merge multiple files into a single file.\n"
            " -c <input_file_path> <output_file_path> Compress a text file into the .goat format.\n"
            " -d <input_file_path> <output_file_path> Decompress a .goat file to retrieve the original text.\n"
            " -q <file_paths>       Print the permissions of one or more files as an integer (000 to 777).\n"
            " -n <source_path> <destination_path> <new_name> Move a file to a new location with a new name.\n"
            " -h                   Print this help page.\n"
        )
        self.assertEqual(result.stdout, expected_help_page, "Help page content does not match expected output")

# Tests for default case
class TestGoatToolDefaultCase(BaseTestGoatTool):
    def test_default_behavior_display_help_page(self):
        result = subprocess.run(['./GoatTool'], capture_output=True, text=True)
        expected_help_page = (
            "GoatTool Help Page:\n"
            " -p <file_paths>       Print the contents of one or more files.\n"
            " -g <file_path> <search_string> Search a file for a particular string and print matching lines.\n"
            " -s <file_paths>       Print the size of one or more files.\n"
            " -m <file_paths> <output_path> Merge multiple files into a single file.\n"
            " -c <input_file_path> <output_file_path> Compress a text file into the .goat format.\n"
            " -d <input_file_path> <output_file_path> Decompress a .goat file to retrieve the original text.\n"
            " -q <file_paths>       Print the permissions of one or more files as an integer (000 to 777).\n"
            " -n <source_path> <destination_path> <new_name> Move a file to a new location with a new name.\n"
            " -h                   Print this help page.\n"
        )
        self.assertEqual(result.stdout, expected_help_page, "Default behavior did not display expected help page")


if __name__ == '__main__':
    unittest.main()
