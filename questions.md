Questions:
----------

- How many bytes are you expecting to recieve in the peek command(including csp headers)
	- Reject first 12 bytes
	- Rest of it is data

- Do you need to send the same number of bytes as that you recieve. And if yes, does that include the bytes in the command?
	- Same.

- Required flags in the peek command, and its effects.
	- Get back.