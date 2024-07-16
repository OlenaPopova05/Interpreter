import subprocess


def run_echo(input_data):
    process = subprocess.Popen(
        ['/Users/olenapopova/Documents/Interpreter/cmake-build-debug/Interpreter'],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True)
    stdout, stderr = process.communicate(input_data)
    return stdout.strip(), stderr.strip()


def test_echo():
    tests = [
        {
            'input': '3 * (8 + 2)\n',
            'expected': '> > 30.000000',
        },
        {
            'input': '3 * (8 + 2) + 1\n',
            'expected': '> > 31.000000',
        },
        {
            'input': '(3 * (8 + 2)) / 5\n',
            'expected': '> > 6.000000',
        },
        {
            'input': 'min(80, max (abs(pow(-3, 3)), 10))\n',
            'expected': '> > 27.000000',
        },
        {
            'input': 'min(13 + 44, pow(2, 3 *2))\n',
            'expected': '> > 57.000000',
        }]

    for test in tests:
        input_data = test['input']
        expected = test['expected']

        output, error = run_echo(input_data)

        if error:
            print(f"Test failed for input:\n{input_data}")
            print(f"Error:\n{error}")
        elif output.strip() != expected:
            print(f"Test failed for input:\n{input_data}")
            print(f"Expected:\n{expected}")
            print(f"Got:\n{output}")
        else:
            print(f"Test passed for input:\n{input_data}")


if __name__ == '__main__':
    test_echo()