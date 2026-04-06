The terminal benchmarks are run with the provided Python harness, Harbor
Installing Harbor is run by creating a Python virtual environment and installing Harbor into this venv
    - python3 -m venv venv
    - source activate venv
    - pip3 install harbor

Docker also needs to be installed as it is used to create environments in which the benchmarks are run
    - brew install docker

To use Harbor with your model of choice, your API key should be a shell variable. The format of the API key must be [MODEL_NAME]_API_KEY
    - export GEMINI_API_KEY=[your API key]

Once everything is installed, the commands I ran to run the benchmarks were the following
    - harbor run --dataset terminal-bench@2.0 --agent gemini-cli --model google/gemini-flash-latest -i git-leak-recovery --agent-timeout-multiplier 4.0

    - harbor run --dataset terminal-bench@2.0 --agent gemini-cli --model google/gemini-flash-latest -i crack-7z-hash --agent-timeout-multiplier 4.0

    - harbor run --dataset terminal-bench@2.0 --agent gemini-cli --model google/gemini-flash-latest -i openssl-selfsigned-cert --agent-timeout-multiplier 4.0

    - harbor run --dataset terminal-bench@2.0 --agent gemini-cli --model google/gemini-flash-latest -i filter-js-from-html --agent-timeout-multiplier 4.0


