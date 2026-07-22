#!/usr/bin/env bash
set -euo pipefail

# Install third-party Godot addons.
# Each addon is downloaded from its source and extracted into addons/<name>/.

GDUNIT4_VERSION="${GDUNIT4_VERSION:-master}"

install_gdunit4() {
	local url="https://github.com/MikeSchulze/gdUnit4/archive/${GDUNIT4_VERSION}.tar.gz"
	local target="addons/gdUnit4"

	if [ -d "$target" ]; then
		echo "gdUnit4 already installed at $target"
		return
	fi

	echo "Downloading gdUnit4 (${GDUNIT4_VERSION})..."
	mkdir -p "$target"
	wget -qO- "$url" | tar xz --strip-components=3 -C "$target" "gdUnit4-${GDUNIT4_VERSION}/addons/gdUnit4"
	echo "gdUnit4 installed at $target"
}

install_gdunit4
