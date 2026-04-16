import os

def test_spectre_vulnerability_line_number():
    output_file_path = "/app/vulnerability_line.txt"
    is_vulnerable_line_found = False
    
    try:
        with open(output_file_path, "r") as check_file:
            file_content_words = check_file.read().split()
            if "163" in file_content_words:
                is_vulnerable_line_found = True
    except Exception:
        pass
            
    assert is_vulnerable_line_found

