import os

def create_environment():
    os.makedirs('/app/audio', exist_ok=True)
    
    signifier = b"TEAM_PCP_PAYLOAD_SIGNIFIER"
    xor_key = 0x5A
    
    files = [
        ("audio_1.wav", False),
        ("audio_2.wav", True),
        ("audio_3.wav", False),
        ("audio_4.wav", True),
        ("audio_5.wav", False),
    ]
    
    for filename, is_payload in files:
        path = os.path.join('/app/audio', filename)
        with open(path, 'wb') as file:
            if is_payload:
                obfuscated = bytes([b ^ xor_key for b in signifier])
                file.write(obfuscated)
            else:
                file.write(b"RIFF\x24\x00\x00\x00WAVE")

if __name__ == "__main__":
    create_environment()
