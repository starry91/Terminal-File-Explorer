# Project Title

A simple file explorer which has 2 modes of exploring files:  
1. Normal Mode: Works like a GUI application with features to scroll, select, navigate and open files  
2. Command Mode: Can be used to explore/take actions using commands

# How to install

Clone the repo and run Make in the project directory

# How it works

The application works in two modes. The application starts in normal mode, which is the default mode and used to explore the current directory and navigate around in the filesystem. 
The root of your application will be the directory where the application was started.  
The last but one line of the display screen is to be used as command bar (when ":" is pressed, a command bar shows up)  
The last line of the display screen is to be used as error bar to show errors.    

## Normal Mode:

Read and display list of files and directories in the current folder.

* Navigation-  
User is able to navigate up & down the file list using corresponding arrow keys  
  
* Open files & directories  
When "ENTER" is pressed-  
Directory​ - Clear the screen and Navigate into the directory  
Files​ - Open files using the corresponding default application(using xdg-open)  
  
* Traversal-  
a. Go Back- On pressing left arrow key the file explorer would go back to the previously visited directory  
b. Go Forward- On pressing right arrow key the file explorer would go forward to the next directory (Similar to forward/back feature of web pages)  
c. Up one level- On pressing backspace goes up one level(parent)  
d. Home - On pressing ‘h’ goes to home/application root directory  

On pressing 'q' in Normal mode, application gracefully exits and the old screen shows up.  

## Command Mode

### Commands available:
1. Copy ':copy <source_file(s)> <destination_directory>'
2. Move ':move <source_file(s)> <destination_directory>'
3. Rename ':rename <old_filename> <new_filename>'
4. Create file ':create_file <file_name> <destination_path>'
5. Create directory ':create_dir <dir_name> <destination_path>'
6. Delete file​ : ':delete_file <file_path>'
7. Delete directory: ':delete_dir <directory_path>'
8. Goto ':goto <location>'
9. Search ​':search <filename>'
10. Command ​':snapshot <folder> <dumpfile>​'

**Paths could be both relative and absolute

On pressing 'ESC' key the application switches to Normal Mode.  

# Things to note:
1. When searched for a name, the search page shows all the files/directories matching with it. Pressing "BACKSPACE" from there takes you to the previous visited page. Pressing "ENTER" opens up the file/dir
2. Snapshot appends to the dumpfile if it already exists

# Authors

    Praveen Balireddy
