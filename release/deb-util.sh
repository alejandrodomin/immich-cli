#!/bin/bash

mkdir -p immich-cli_0.alpha/usr/local/bin
mkdir -p immich-cli_0.alpha/DEBIAN

cp release/control immich-cli_0.alpha/DEBIAN/
cp immich-cli immich-cli_0.alpha/usr/local/bin/
chmod 755 immich-cli_0.alpha/usr/local/bin/immich-cli
