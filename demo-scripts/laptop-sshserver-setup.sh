#!/bin/bash

sudo systemctl enable ssh
sudo systemctl start ssh

sudo ufw default deny incoming
sudo ufw allow from 10.42.0.0/24 to any port 22 proto tcp   # or 10.42.0.30/32
sudo ufw enable
sudo ufw status verbose