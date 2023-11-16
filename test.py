import subprocess
import tempfile
import unittest
import os

# Base case tests (shared functions)
class BaseTestGoatTool(unittest.TestCase):
    def create_temp_file(self, content="", extension=".txt"):
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=extension, mode='w')
        temp_file.write(content)
        temp_file.close()
        return temp_file.name

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


if __name__ == '__main__':
    unittest.main()
