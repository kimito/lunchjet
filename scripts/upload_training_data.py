if '__file__' in globals():
    import os, sys
    sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from lunchbox import GoogleDrive
import time
import tarfile

def main():

    tarball_name = "train_{}.tar.gz".format(time.strftime('%Y_%m_%d_%H_%M_%S'))

    with tarfile.open(tarball_name, "w|gz") as archive:
        archive.add("./")

    print('create {}'.format(tarball_name))

    gdrive = GoogleDrive(client_secret_file='credentials.json', token_file='token.json')

    file = gdrive.create_file('lunchbox/' + tarball_name, tarball_name)
    print('upload a file to gdrive : {}'.format(str(file)))

if __name__ == '__main__':
    main()

