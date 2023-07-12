#!/bin/bash

#color
GREEN='\033[0;32m'
RESET='\033[0m'
RED='\033[0;31m'


# IRC server details
SERVER="127.0.0.1"
PORT=8080

# Connect to the IRC server
echo "Connecting to $SERVER:$PORT"
exec 3<> /dev/tcp/$SERVER/$PORT

# Function to send commands to the IRC server
send_command() {
  echo "$1"
  echo "$1" >&3
}

test="test.txt"
result="result.txt"

while IFS= read -r line
do
  send_command "$line"
done < "$test"

# Read responses from the IRC server
line_number=1
clean_string = "\n"
echo "Specific line: $specific_line"
while IFS='' read -r response; do
  specific_line=$(sed "${line_number}q;d" "$result")
  if [[ "$response" == "$specific_line\n" ]]; then
    echo -e "${GREEN}✅ ok ${RESET}"
else
    echo -e "${RED}❌ ko${RESET}"
    ((line_number++))
  fi
done <&3

# Close the connection
exec 3>&-





# Retrieve the specific line from the file using sed
