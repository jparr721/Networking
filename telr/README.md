 ```
   ______     __                
  /_  __/__  / /           _____
   / / / _ \/ /  ______   / ___/
  / / /  __/ /  /_____/  / /    
 /_/  \___/_/           /_/     
                               


 ```
python cron task to get the IP address of your tiny computer when you're hacking in the field.

If you have access to your router's DHCP lease allocation table this isn't really needed. This is only for when you're, for instance, trying to initate some sort of attack on a public/private wifi network. You would ideally gain entrance to the network, log in with this computer, and then do some stuff.

Commonly when I am out doing stuff I want to get the ip from my rpi3. This helps do that without needing a display to check manually.

## Compiling
### Requirements
* Python 3 (2 is no good)
* Preferably being hard-wired to a network, wifi autoconnect is experimental
* Google docs API and Google Drive API
* This project depends on `client_secret.json` which is renamed from the credential file you get when you register for the API
* (For now) a debian based system (tested on rpi3)

All other dependencies will be handled by the install script

### Steps
`chmod 777 setup.sh`

`./setup.sh`

`python(3) setup.py`

`telr [options]`
