#!/bin/sh
# Environmental variable(s)
currentWorkingDir=$(pwd)
projectName=Test

# Executing the command from the make file
make cmakeClean

# Creating the directory
mkdir -m 777 build 2>/dev/null
cd build

cmake ../ && make

# If the command, cmake ../ && make, works, ...
if [ $? -eq 0 ]; then

	#Linux version
	if [ $1 -eq 0 ]; then
		# Entering the unit test folder
		cd Tests
		sudo chown root:root $projectName
		sudo chmod 4755 $projectName
		#./$projectName

	# Windows version
	else
		# Entering the unit test folder
		cd Tests
		./$projectName.exe
	fi
fi
cd $currentWorkingDir
