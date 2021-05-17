# Copyright 2018 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

from __future__ import print_function
import os.path
from googleapiclient.discovery import build
from google_auth_oauthlib.flow import InstalledAppFlow
from google.auth.transport.requests import Request
from google.oauth2.credentials import Credentials
from googleapiclient.http import MediaFileUpload

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/drive']

#mime type of Google Drive's directory
DIR_MTYPE = 'application/vnd.google-apps.folder'

class GoogleDrive:
    def __init__(self, client_secret_file, token_file):
        self.creds = self._get_cred(client_secret_file=client_secret_file, token_file=token_file)
        self.service = build('drive', 'v3', credentials=self.creds)
    
    def _get_cred(self, client_secret_file, token_file):
        creds = None
        if os.path.exists(token_file):
            creds = Credentials.from_authorized_user_file(token_file, SCOPES)
        if not creds or not creds.valid:
            if creds and creds.expired and creds.refresh_token:
                creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file(
                    client_secret_file, SCOPES)
                creds = flow.run_local_server(port=0)
            with open(token_file, 'w') as token:
                token.write(creds.to_json())
        
        return creds

    def create_file(self, name, content_file_name=None, *, mime_type=None, parent_dir_id=None):
        paths = name.split('/')
        file_name = paths[-1]
        dir = '/'.join(paths[:-1])
        body = {'name' : file_name}
        if dir is not None and len(dir) > 0:
            parent_dir_id = self.get_directory(dir, parent_dir_id=parent_dir_id).get('id')
        if parent_dir_id is not None:
            body['parents'] = [parent_dir_id]
        if content_file_name is None:
            content_file_name = name
        media = MediaFileUpload(content_file_name, mimetype=mime_type, resumable=True)
        file = self.service.files().create(
            body = body,
            media_body = media,
            media_mime_type = mime_type
        ).execute()
        return file

    def create_directory(self, name, *, parent_dir_id=None):
        body = {
            'name' : name,
            'mimeType': DIR_MTYPE
        }
        if parent_dir_id is not None:
            body['parents'] = [parent_dir_id]
        file = self.service.files().create(body = body).execute()
        return file

    def find_files(self, name, *, mime_type=None, parent_dir_id=None):
        qs = [
            "name = '{}'".format(name),
            "trashed = false"] #exclude entries in trash
        if mime_type is not None:
            qs.append("mimeType='{}'".format(mime_type))
        if parent_dir_id is not None:
            qs.append("'{}' in parents".format(parent_dir_id))
        q = None
        if len(qs) > 0:
            q = ' and '.join(qs)
        page_token = None
        files = []
        while True:
            response = self.service.files().list(q=q,
                                                spaces='drive',
                                                fields='nextPageToken, files(id, name)',
                                                pageToken=page_token).execute()
            for file in response.get('files', []):
                files.append(file)
            page_token = response.get('nextPageToken', None)
            if page_token is None:
                break
        return files

    def get_directory(self, name, *, parent_dir_id=None, force_create=True):
        dir_names = name.split('/')
        ret = None
        pdi = parent_dir_id
        for dir_name in dir_names:
            dir = self.find_files(dir_name, mime_type=DIR_MTYPE, parent_dir_id=pdi)
            if dir is not None and len(dir) > 0:
                ret = dir[0]
            else:
                if force_create is True:
                    ret = self.create_directory(dir_name, parent_dir_id=pdi)
                else:
                    return None
            pdi = ret['id']
        return ret
        

def main():

    gdrive = GoogleDrive(client_secret_file='credentials.json', token_file='token.json')

    file = gdrive.create_file('lunchbox/annotations/test.txt', 'test.txt')
    print(str(file))

if __name__ == '__main__':
    main()
