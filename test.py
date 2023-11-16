import subprocess
import tempfile
import unittest
import os

# Tests for -p switch
class TestGoatToolPrintFunction(unittest.TestCase):

    def create_temp_file(self, content="", extension=".txt"):
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=extension, mode='w')
        temp_file.write(content)
        temp_file.close()
        return temp_file.name

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

if __name__ == '__main__':
    unittest.main()
