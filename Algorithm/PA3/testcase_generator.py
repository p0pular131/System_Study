import random
import string
import sys

# Default parameters
DEFAULT_LENGTH = 100  # 기본 텍스트 길이
OUTPUT_FILE = "hw3_input.txt"  # 출력 파일 이름

def generate_test_file(filename, length):
    """
    지정된 길이만큼의 랜덤 텍스트 파일을 생성합니다.
    
    Args:
        filename (str): 생성할 파일 이름
        length (int): 생성할 텍스트의 길이
    """
    print(f"Generating test file '{filename}' with length {length}...")
    try:
        with open(filename, "w") as file:
            for _ in range(length):
                random_char = random.choice(string.printable[:95])  # ASCII 32~126
                file.write(random_char)
        print(f"Test file '{filename}' generated successfully.")
    except Exception as e:
        print(f"Error generating file: {e}")

if __name__ == "__main__":
    # 기본 길이를 사용하거나, 명령줄 인자로 길이 지정
    length = DEFAULT_LENGTH
    if len(sys.argv) > 1:
        try:
            length = int(sys.argv[1])
            if length <= 0:
                raise ValueError("Length must be greater than 0.")
        except ValueError as ve:
            print(f"Invalid length specified: {ve}")
            print(f"Using default length {DEFAULT_LENGTH}.")
            length = DEFAULT_LENGTH

    # Test file 생성
    generate_test_file(OUTPUT_FILE, length)
