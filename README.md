Table of Contents
=================

   * [cbMarkdown](#cbmarkdown)
      * [Features and limitations](#features)
      * [Screen shots](#screen_shots)
      * [Getting Started](#getting_started)
         * [Prerequisites](#prerequisites)
         * [Installing](#installing)
      * [Using the plugin](#using_the-plugin)
         * [Prerequisites](#prerequisites_1)
         * [Set up the project](#set_up_the_project)
         * [User interface](#user_interface)
         * [Debugging and writing register](#debugging_and_writing_register)
      * [Contributing](#contributing)
      * [Changelog](#changelog)
      * [Authors](#authors)
      * [License](#license)
      * [Acknowledgments](#acknowledgments)



# cbMarkdown <a name="cbmarkdown"></a>

A markdown render preview plugin for [Code::Blocks IDE](http://www.codeblocks.org/).


This plugin allows you to render a preview of [markdown](https://daringfireball.net/projects/markdown/)-files and displayed in a property grid.
This plugin uses the [discount](http://www.pell.portland.or.us/~orc/Code/discount/) markdown to html library. For supported sysntax see the discount webpage.

## Features and limitations <a name="features"></a>
 * This plugin uses the [discount](http://www.pell.portland.or.us/~orc/Code/discount/) library to render the markdown to html. So all features and limitations of this library apply to this plugin also.
 * The plugin uses the system provided web rendering ( for ex. IE7 on Windows) or the wxWidgets provided renderer. The later does not provide CSS support
 * Embed images do not render at the moment
 * External Links are opened in the system browser.
 * Local (file internal links) work only if you use the `<a name="index"></a>` syntax


## Screen shots <a name="screen_shots"></a>
Windows 7 with native renderer
![screen shot windows 7 native](screenshots/)

Windows 7 with wxWidgets renderer
![screen shot windows 7 wxWidgets](screenshots/)

Xubuntu with native renderer
![screen shot Xubuntu native](screenshots/)

Xubuntu with wxWidgets renderer
![screen shot Xubuntu wxWidgets](screenshots/)

The plugin settings window on windows 7
![Screen shot register view windows](screenshots/)

## Getting Started <a name="getting_started"></a>

At the moment there are no binary builds available for this plugin so you have to build it by yourself.

### Prerequisites <a name="prerequisites"></a>
#### Codeblocks
You need the source of codeblocks and install it by hand:
An instruction to build Code::Blocks can be found [here](http://wiki.codeblocks.org/index.php/Installing_Code::Blocks)
After the build and installation of codeblocks you should have a folder with the codeblocks source similar to this:
```
/home/test/programming/codeblocks_source/
```
In this folder there should be a folder called *src*, *m4* and *debian*. The Code::Blocks source code should be found under *src*

#### wxWidgets
A installed and running wxWidgets library with version 3.X.X (you need this also for building Code::Blocks)

#### discount <a name="discount_installation"></a>
You need the [discount](http://www.pell.portland.or.us/~orc/Code/discount/) library on your system.
Please read the README found in the discount folder for instructions to build it on your system.

After the installation of the discount library there should be the following folder on your system

````
discount installation directory (provided by --prefix switch during configure)
|---bin
|       markdown
|---include
|       mkdio.h
|---lib
    |   libmarkdown.a
    |---pkgconfig
````


#### cbMarkdown
To get the code for this plugin you can clone the git repo or download the code as zip archive.

### Installing <a name="installing"></a>

0. Build a Code::Blocks version with wxWidgets version 3.X.X

1. To build the plugin simply open the project file corresponding for your operating system:

 + windows: cbMarkdown.cbp
 + linux: cbMarkdown_unix.cbp
 + mac: Sorry, i don't have a mac to provide any project file... Help is welcome...

 After opening the project codeblocks will ask you about two global variables:
 
**cb**: this is the source ditectory of codeblocks. Set _base_ to your codeblocks source folder
```
/home/test/programming/codeblocks_source/src
```
**discoutn**: Configure the  global variable to point to the [*installation*](#discount_installation) folder of your discount repository

You can configure the global variable later under:
  * Settings->Global variables
  * Select cb on the left list
  * Set the path in the *base* field

3. select the *cb_plugin_wx30* target from the drop down menu near the build button
4. Hit the build button
5. Fix errors (there should not be any errors ;) )
6. Open your productive codeblocks program
7. Plugins->Manage plugins->Install new
8. Browse to
```
/home/test/programming/cbSystemView_source/plugin_wx30
```
and select the plugin to install.

  **!!NOTE**: The plugin can only run in a Code::Blocks version compiled with the same compiler and the same wxWidgets version.


## Using the plugin <a name="using_the_plugin"></a>
You can activate the window by the menu View->Markdown view
If you open an editor tab with a file that has a registered file [ending](#setup_filter) the view window will show you the rendered markdown file.
By default registered ist the _.md_ ending. After Switching tab the view remains in the last markdown file.

### Set up file filter<a name="setup_filter"></a>
You can customize the file ending trigger in the menu `Settings->Editor->cbMarkdown->File extensions`. Enter the file ending of the file without `.` and separated by a semicolon `;`.

### Set up render engine<a name="setup_renderer"></a>
If you have problems with the render engine you can switch it with `Settings->Editor->cbMarkdown->Render technology`:
* **Html view**: The wxWidgets internal render engine is used. No CSS support or complicated rendering
* **Web view**: The operating system default rendering engine is the used to render the HTML page. Under Windows the IE7 Engine is used (see [wxWidgets documentation](https://docs.wxwidgets.org/trunk/classwx_web_view.html))

### Set up custom css<a name="setup_renderer"></a>
You can set up custom CSS renderer in `Settings->Editor->cbMarkdown->CSS used for webview`. This takes only affect if the *Web view* renderer is active. You can reset to the plugin default css with the button _Reset CSS_

## Contributing <a name="contributing"></a>

Any bug report, new idea, patch, pull requests are welcome.

## Changelog  <a name="changelog"></a>

No actual release

## Authors <a name="authors"></a>

* **bluehazzard** - *Initial work*


## License <a name="license"></a>

This project is licensed under the GPL v3 License - see the [LICENSE.md](LICENSE) file for details

## Acknowledgments <a name="acknowledgments"></a>

The whole Code::Blocks team

