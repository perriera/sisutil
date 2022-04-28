## How to clone your project with (perriera) extras_cpp template
> This is a basic step-by-step process whereby you can quickly setup a C++17 based project using CMake 3.21 (or above) including a properly setup changelog.md file (that will help you keep track of changes to your github.com repository).

 1. **GIVEN** we need to create a C++17 template with CMake 3.2.1
 2. **WHEN** we use (perriera) extras_cpp as that template
 3. **THEN** we can just make a few changes to have our C++ project

### Prerequisites
 1. Github.com account (or something similiar)
 2. Access to *(perriera) extras_cpp* 
 3. The ability to create a project from this template
 4. You are logged into your account
 5. You are running Linux (or a variant) 
 6. You have the git utility installed
 7. You have Visual Studio Code installed (or similar) 
 8. You have a SSH key added to your github.com account

### Wish Case
As you already using github.com it therefore perfectly logical for you to merely press the green NEW button to the left of your github.com page. 

 1. Go to your github.com account page 
 2. Press the green button entitled *New*
 3. Under *Repository template s*elect *perrier/extras_cpp*
 4. In the *Repository name* field enter the name of your project
 5. Optionally fill in the *Description* field
 6. Select *Public* or *Private*
 7. Check over the parameters (for accuracy)
 8. Press the green Create Repository button
 9. Now you will be shown your new project
 10. There should be a green Code button, click it
 11. Select the SSH tab (default)
 12. Copy the URL to the clipboard
 13. Open a Terminal box
 14. Make/create a projects directory
 15. Cd into that directory
 16. Execute **git clone <url>** (paste the clipboard)
 17. cd into your projects directory
 18. Execute **code .** 
 19. Now you need to do a simple search and replace
 20. In Visual Studio Code top menu do  *Edit->Replace In Files*
 21. Highlight the *Aa* option as this is going to be case sensitive
 22. Replace *extras_cpp* with *<your_project_name>* (all lowercase)
 23. (You will see a small icon to the right of the replace field)
 24. Replace *EXTRAS_CPP* with *<YOUR_PROJECT_NAME>* (all caps)
 25. Click the replace icon again
 26. Now click on the Visual Studio Code Explorer window 
 27. Open up the include directory
 28. Change *extras_cpp* directory name to your *<your_project_name>*

From here you have created a new project with the **(perriera) extras** template for C++17. 

### Alternate Case
#### In the case of not having a github.com account
 1. Goto Github.com 
 2. In the *Search Github* text box enter *extras_cpp*
 3. Scroll down until you see *perrier/extras_cpp*
 4. Click on  *perrier/extras_cpp*
 5. There should be a green Code button, click it
 6. Select the HTTPS tab (default)
 7. Copy the URL to the clipboard
 8. Open a Terminal box
 9. Make/create a projects directory
 10. Cd into that directory
 11. Execute **git clone <url>** (paste the clipboard)
 12. Execute **mv extras_cpp <name of your project>**
 13. Execute **code .** 

### Alternate Case
#### You downloaded the .zip file instead
 1. There should be a green *Code* button, click it
 2. Press the *Download ZIP* option
 3. In your *~/Downloads* folder you should see the zip
 4. Cut & Paste that to your projects directory
 5. Unzip the file 
 6. Open a Terminal box
 7. Make/create a projects directory
 8. Cd into that directory
 9. Execute **git clone <url>** (paste the clipboard)
 10. Execute **mv extras_cpp-dev <name of your project>**
 11. Execute **code .** 

### Summary 
This has been a basic example of how to clone the *(perriera) extras_cpp* repository (and rename it for your purposes). However, you are not really done yet. You will probably want to change all the *extras_cpp* references to your project name.

### Next Steps
 - [How to setup your project with (perriera) extras_cpp template](https://github.com/perriera/extras_cpp/blob/dev/docs/SETUP.md)
- [How to install C++17, CMake, PIP and other tools for your project](https://github.com/perriera/extras_cpp/blob/dev/docs/INSTALL.md)

### Copyright (see [LICENSE.md](https://github.com/perriera/extras_cpp/blob/dev/LICENSE.md))
> The "MIT/X Consortium License", (adapted for EXPARX.COM)<br/>
> Copyright (C) November 22, 2021 EXPARX INCORPORATED<br/>
> Markup Language created/edited with [SlackEdit.io](https://stackedit.io/app#)<br/>
> CPM support made possible by [Matt Williams](Matt@programmer)<br/>

