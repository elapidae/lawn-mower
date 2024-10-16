#!/bin/bash -e

mkdir -p pushik-raspi
sshfs -p2223 pushik@$vdsina:/home/pushik pushik-raspi

