if '__file__' in globals():
    import os, sys
    sys.path.append(os.path.join(os.path.dirname(__file__), '..'))

from lunchjet import GoogleDrive
import time
import tarfile
from argparse import ArgumentParser
import re
import pathlib


def unlink_files_in(dir_path):
    for file in dir_path.glob('**/*'):
        if file.is_file():
            file.unlink()


def main():
    parser = ArgumentParser(description="create tarball and upload it to Google Drive")
    parser.add_argument('files', metavar='FILE', type=str, nargs='+', help='files to send')
    parser.add_argument('--root', metavar='ROOT_DIR', type=str, nargs='?', default=None,
     help='root directory of the archive. files in the archive is put in relateve path from this directory. this is usable if FILE(s) is absolute path')
    parser.add_argument('--rm', action='store_true', help='delete sent files. if FILE is a directory, only files in the directory are to be deleted.')
    args = parser.parse_args()
    
    if args.root is None:
            args.root = str()
    if args.root[-1] is not '/':
        args.root += '/'

    # create tarball
    tarball_name = "train_{}.tar.gz".format(time.strftime('%Y_%m_%d_%H_%M_%S'))
    with tarfile.open(tarball_name, "w|gz") as archive:
        for file in args.files:
            relative_path = re.sub('^{}'.format(args.root), '', file)
            archive.add(file, arcname=relative_path)
    print('create {}'.format(tarball_name))

    # upload the tarball to Google Drive
    gdrive = GoogleDrive(client_secret_file='credentials.json', token_file='token.json')
    file = gdrive.create_file('lunchbox/' + tarball_name, tarball_name)
    print('upload a file to gdrive : {}'.format(str(file)))

    # delete files if needed
    if args.rm:
        for file in args.files:
            file_path = pathlib.Path(file)
            if file_path.is_dir():
                unlink_files_in(file_path)
            else:
                file_path.unlink()


if __name__ == '__main__':
    main()

