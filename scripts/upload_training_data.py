if '__file__' in globals():
    import os, sys
    sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from lunchbox import GoogleDrive

def main():

    gdrive = GoogleDrive(client_secret_file='credentials.json', token_file='token.json')

    file = gdrive.create_file('lunchbox/annotations/test.txt', 'test.txt')
    print(str(file))

if __name__ == '__main__':
    main()

