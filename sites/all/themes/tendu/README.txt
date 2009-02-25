Documentation for Tendu theme version 2.x

Last update:            26 Jan 2009.
Author:                 Tom Bigelajzen (http://drupal.org/user/173787) 
Author homepage:        http://tombigel.com
Project URL:            http://drupal.org/project/tendu
Full Documentation URL: soon...
Demo site URL:          http://tendu.tombigel.com 
                        (Drupal 5, Tendu 1.5, will be upgraded soon to Drupal 6 
                         and the latest Tendu 2.x)

Introduction:
   Tendu is the sum of my 5 years experience in HTML and CSS development for
   Bi-Directional websites, 2 of them using Drupal.
   It was born from the lack of a light, stable, table-less and cross-browser 
   theme that support RTL and BiDi layout "out of the box". 
   Tendu has a lot of the useful features that can be found in other "Starter
   themes" like Zen (I copied or re-wrote some of Zen's features) but is much
   less complicated and bloated then these themes, though a bit less friendly 
   for the non-professional user.

Main Features:
   * Table-less design, supports 1,2 or 3 columns, fixed or flexible width layouts.
   * Full support for multi-language, left-to-right and right-to-left websites.
     - Said to have the best RTL support out of all the starter themes 
     (http://drupalstaging.com/starter-themes/starter-theme-comparison.html).
   * Cross-browser support for all major browsers, and reported to work on some of 
     the older or more exotic ones (Among them IE4, IE5 and Amaya).
   * Cross-browser support for 100% body height with the footer sticked to the bottom 
     of the viewport on short pages.
   * Accessibility:
     - Relative font size and well formed page structure for easy font size change and 
       for fluent page read in screen reader.
     - Hidden links that pop on focus to the content at the top and bottom of the page 
       for screen readers and keyboard-only navigation
     - Hidden headers to menus to comply with accessibility guidelines
   * Degradable design for older and non-standard browsers to avoid the use of CSS hacks.
   * Integrated "conditional-styles" module, for easy addition of IE conditional 
     comments through the theme's .info file.
   * Automatically adds the "Language Switcher" block when "locale" module is 
     installed and more then one language is defined.
   * Integrated search box, primary and secondary menus.
   * Features 9 dynamic content regions for flexible design.
   * Sub-theme support, inside or outside the base theme.
   * CSS3 round corners for Mozilla/Webkit browsers.
   * Built in support for Block Class and Block Theme modules in block.tpl
   * Theme Settings form to toggle the Language Switcher and Accessibility links
   * More... 

Installation:
   1. Download the latest Tendu version 2.x from http://drupal.org/project/tendu
   2. Unpack the .tar.gz file into your /sites/all/themes directory.
   3. in www.example.com/admin/build/themes select "Tendu" or one of it's 
      sub-themes
   4. Go to theme settings and check/uncheck the theme properties, note the 
     "Language Switcher" box at the bottom of the settings page.

Usage:
   
1. The theme structure:
   
   Tendu has 2 parts - The base theme and the sub-themes.
   The main theme defines the basic CSS, template functions and structure.
   The sub-themes define the design, and can override the base theme features.
   
   Right now Tendu has 2 sub-themes - 
   
    * Tendu Default Design:
      A basic clean sub-theme that demonstrates what can be done with Tendu,
      can be used as a stand-alone theme or as a base to develop other themes 
      on.
    
    * Doil (In Development, not ready for production yet) 
      A theme developed for http://drupal.org.il - The home for Drupal  
      developers and users in Israel.
   
   In Tendu I use the concept of sub-theming a bit differently:
   Tendu itself is not intended to be used as a standalone theme, but 
   as a core for advanced layouts (= sub-themes) to be built on.   
    
2. Quick start:   
   
   Note that at least a basic knowledge in CSS is recommended to costume Tendu. 
   
   After installing Tendu, select "Tendu Default Design" in /admin/build/
   themes.

   In Theme Settings->Tendu Default Design, make sure "Primary Links",
   "Secondary Links", "Search Box" (requires the Search module) and 
   "Language Switcher" (requires the locale modules, shows only when more
   then one language is defined for the site) are selected.
   
   Open style.css in an editor and follow the instructions in the comments.

   There are four important sections there:

   - "Full Viewport Height" and "Stick footer to bottom of the page" 
     for sticking the footer to the bottom of the window in pages with short content. 
     A nice to have feature.
     * Note that this definition sets a fixed height to the footer so blocks in the 
       footer will be hidden if height is not set correctly

   - "Fonts" 
     for setting a global font size other then 12px or font family other then
     Ariel, Helvetica, sans-serif
   
   - "Page Width" 
     for setting a fixed of flexible page width, with support for
     fixed or 100% page width in IE6 or lower.

   - "Sidebar Widths" 
     is most likely the only part in this file that everybody need to edit.
     * Note that any change to the sidebars widths or paddings must be edited in
       ie6.css and in style-rtl.css and ie6-rtl.css if the site is BiDi or RTL. 
  
   Thats it. If you want to use Tendu Default Design as-is, these are the only
   settings you need to change.
   
   One more thing...
   
   If you edit style-subtheme.css in /tendu_default folder you'll find a large 
   commented area at the end.
   If you uncomment this part, your site will have rounded corners in modern 
   Mozilla and Webkit browsers. 