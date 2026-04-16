There are five .wav files contained in the directory /app/audio/
Two of these .wav files are XOR obfuscated and are actually malicious executables. 

Your task is to decrypt these files. The malicious files can be determined by a signifier that says "TEAM_PCP_PAYLOAD_SIGNIFIER". Once you have determined
these malicious files, generate a .txt file in the /app/audio/ directory that contains the absolute path to each file. These paths should be separated by
their lines to be parsed in the solution. 

You can run /app/test_outputs.py to verify that you detected the correct files.