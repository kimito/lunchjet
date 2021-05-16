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

# If modifying these scopes, delete the file token.json.
SCOPES = ['https://www.googleapis.com/auth/drive']

class GoogleDrive:
    def __init__(self, client_secret_file, token_file):
        self.creds = self._get_cred(client_secret_file=client_secret_file, token_file=token_file)
        self.service = build('drive', 'v3', credentials=self.creds)
    
    def _get_cred(self, client_secret_file, token_file):
        creds = None
        # The file token.json stores the user's access and refresh tokens, and is
        # created automatically when the authorization flow completes for the first
        # time.
        if os.path.exists(token_file):
            creds = Credentials.from_authorized_user_file(token_file, SCOPES)
        # If there are no (valid) credentials available, let the user log in.
        if not creds or not creds.valid:
            if creds and creds.expired and creds.refresh_token:
                creds.refresh(Request())
            else:
                flow = InstalledAppFlow.from_client_secrets_file(
                    client_secret_file, SCOPES)
                creds = flow.run_local_server(port=0)
            # Save the credentials for the next run
            with open(token_file, 'w') as token:
                token.write(creds.to_json())
        
        return creds

    def create_file(self, name, content_file_name=None, mime_type=None):
        if(content_file_name is None):
            content_file_name = name
        file = self.service.files().create(
            body = {'name' : name},
            media_body = content_file_name,
            media_mime_type = mime_type
        ).execute()
        return file

    def get_directory(self, name, *, parent_dir_id=None, force_create=True):
        body = {
            'name' : name,
            'mimeType': 'application/vnd.google-apps.folder'
        }
        if(parent_dir_id is not None):
            body['parents'] = [parent_dir_id]

        file = self.service.files().create(body=body).execute()
        return file

        
def main():

    gdrive = GoogleDrive(client_secret_file='credentials.json', token_file='token.json')
    file = gdrive.create_file('test.txt')
    print(str(file))

if __name__ == '__main__':
    main()
