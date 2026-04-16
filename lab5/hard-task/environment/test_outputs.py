import os
import glob

def test_spectre_vulnerability_line_number():
    workspace_directory = os.getcwd()
    text_files = glob.glob(os.path.join(workspace_directory, "**", "*.txt"), recursive=True)
    
    is_vulnerable_line_found = False
    for file_path in text_files:
        try:
            with open(file_path, "r") as check_file:
                file_content_words = check_file.read().split()
                if "163" in file_content_words:
                    is_vulnerable_line_found = True
                    break
        except Exception:
            pass
            
    assert is_vulnerable_line_found

