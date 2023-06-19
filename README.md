# interactiveHat
Motor driven animatronic, self and manual controls

Motors driven by arduino code
motorController.ino
  3 motors
    motor 1 swivels base
    motor 2 open/close mouth
    motor 3 controls top of hat
  Backup random ambient motion if PI signal lost
  


Commanded by py code on raspi 4
hatCommander.py
  Two Modes
    Mode 1 Automatic
      Ambient motion and passive voice lines
    Mode 2 manual control
      Controlled by physical controller
      voice lines manually selected and played over bluetooth connection
    Mode 3 phone control
      Select voice line
        Plays audio
        moves along with audio

to run the hatcommander service to autorun on pi startup

sudo cp hatCommander.service /lib/systemd/system/
sudo chmod 644 /lib/systemd/system/hatCommander.service
chmod +x /home/pi/hatCommander.py
sudo systemctl daemon-reload
sudo systemctl enable hatCommander.service
sudo systemctl start hatCommander.service