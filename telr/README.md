# Telr
python cron task to get the IP address of your tiny computer when you're hacking in the field.

Commonly when I am out doing stuff I want to get the ip from my rpi3. This helps do that without needing a display to check manually.

## Compiling
### Requirements
* Python 3 (2 is no good)
* Google docs API and Google Drive API
* This project depends on `client_secret.json` which is renamed from the credential file you get when you register for the API

All other dependencies will be handled by the install script

### Steps
`chmod 777 setup.sh`

`./setup.sh`

`python(3) setup.py`

`telr [options]`
