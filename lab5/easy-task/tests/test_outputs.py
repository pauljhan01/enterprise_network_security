import os

def test_malicious_files_output():
    output_file = "/app/audio/malicious_files.txt"
    assert os.path.exists(output_file), f"Output file {output_file} not found"
    
    with open(output_file, "r") as f:
        lines = f.read().splitlines()
        
    extracted_paths = set(line.strip() for line in lines if line.strip())
    expected_paths = {"/app/audio/audio_2.wav", "/app/audio/audio_4.wav"}
    
    assert extracted_paths == expected_paths, f"Expected {expected_paths}, but found {extracted_paths}"

if __name__ == '__main__':
    test_malicious_files_output()
    print("Test passed successfully!")
