Requirements
------------
Drupal 6.x	  -> http://www.drupal.org
Yahoo YUI     -> http://yui.yahooapis.com/2.5.0/
  		
What is YUI -> The Yahoo! User Interface Library
------------------------------------------------
This module is for taking the advantage of available Yahoo YUI components stuff.
Has alot of components but for now only the drop down menu is implemented.
  
Installation
------------
- The is two different way of how to consume the yui library.  
  1. Hosting approach (default)
     Uses the required yui library directly form http://yui.yahooapis.com servers
  2. Local approach
     Download the yui library.
     Create a directory and extract the library. (this path must be accesible from browsers) 
- Login as an administrator. Enable the module in the "Administer" -> "Site building" -> "modules"
- Edit the settings under "Administer" -> "Site configuration" -> "YUI Common Settings".
- If you like to use the library locally enter the location of your library.
  If you extract the library under files/lib so enter just files/lib to "Location of YUI library".