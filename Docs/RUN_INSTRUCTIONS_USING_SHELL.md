# How to Compile and Run MapDriver with a Shell Script 🚀

You can automate compiling and running your MapDriver program using a shell script. This is especially useful for repeated testing!

## Example Shell Script

Create a file named `run.sh` in your project directory with the following content:

```sh
#!/bin/zsh

echo "🔨 Compiling MapDriver.cpp..."
g++ -std=c++17 -o MapDriver MapDriver.cpp Models/Map.cpp

if [[ $? -eq 0 ]]; then
	echo "🚀 Running MapDriver..."
	./MapDriver
else
	echo "❌ Compilation failed!"
fi
```

## How to Use

1. Save the script above as `run.sh` in your project directory.
2. Make it executable: ( Once you run this cmd, skip to step 3 everytime you want to run the script)
   ```sh
   chmod +x run.sh
   ```
3. Run the script:
   ```sh
   ./run.sh
   ```

## Giving Execute Permission to Other Scripts

If you have other shell scripts (for example, `run_player_driver.sh`), you also need to give them execute permission before running:

```sh
chmod +x run_player_driver.sh
```

Then you can run it with:

```sh
./run_player_driver.sh
```

This will compile and run your program, showing emoji status messages for each step!
