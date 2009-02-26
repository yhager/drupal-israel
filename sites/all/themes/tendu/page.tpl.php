<?php 
// $id$
/**
 * Tendu Drupal - A CSS Theme For Developers
 * Author: Tom Bigelajzen (http://drupal.org/user/173787) - http://tombigel.com
 * Initial Drupal 6 porting: 
 *   Lior Kesos (http://drupal.org/user/41517)
 *   Zohar Stolar (http://drupal.org/user/48488) 
 *   http://www.linnovate.net
 */
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
    
  </head>
  <body class="<?php print $body_classes; ?>">    
    <div id="page">
      <div id="header">        
        
        <?php if ($accesibility_links):?>
        <a name="top" class="accessibility-link" href="#after-header"><?php print t('[Skip Header and Navigation]');?></a>
        <a class="accessibility-link" href="#content-anchor"><?php print t('[Jump to Main Content]');?></a>
        <?php endif;?>
        
        <div id="header-content"> 
          
          <div id="site-details">
            <?php if ($logo): ?>
            <<?php ($is_front && !$site_name) ? print 'h1' : print 'div'; ?> id="site-logo">
              <a href="<?php print $front_page; ?>" name="home" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a>
            </<?php ($is_front && !$site_name) ? print 'h1' : print 'div'; ?>>
            <!-- /logo -->
            <?php endif; ?>
            
            <?php if ($site_name): ?>
            <<?php ($is_front) ? print 'h1' : print 'div'; ?> id='site-name'>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
                <?php print $site_name; ?>
              </a>
            </<?php ($is_front) ? print 'h1' : print 'div'; ?>>
            <!-- /site-name -->
            <?php endif; ?>
            
            <?php if ($site_slogan): ?>
            <blockquote id='site-slogan'>
              <?php print $site_slogan; ?>
            </blockquote>
            <!-- /site-slogan -->
            <?php endif; ?>
            
          </div>
          <!-- /site-details -->
          
          <?php if (!empty($language_switcher) or !empty($search_box) or !empty($header)):?>
          <div id="header-blocks" class="region">            
            
            <?php if (!empty($language_switcher)): ?>
            <div id="language-switcher"><?php print $language_switcher; ?></div>
            <!-- /language-switcher-->
            <?php endif; ?>
            
            <?php if (!empty($search_box)): ?>
            <div id="search-box"><?php print $search_box; ?></div>
            <!-- /search-box -->
            <?php endif; ?>

            <?php if (!empty($header)) print $header; ?>
          
          </div>          
          <!-- /header-blocks -->  
          <?php endif; ?>
          
        </div>        
        
        <?php if (!empty($primary_links) or !empty($secondary_links) or !empty($main_nav)): ?>   
        <div id="main-nav" class="region">
          
          <?php if (!empty($primary_links)): ?>         
          <h2 class="primary-links-title"><?php print t('Primary Links');?></h2>
          <?php print theme('links', $primary_links, array('class' => 'menu primary-links')); ?>         
          <?php endif; ?>
          
          <?php if (!empty($secondary_links)): ?>
          <h2 class="secondary-links-title"><?php print t('Secondary Links');?></h2>         
          <?php print theme('links', $secondary_links, array('class' => 'menu secondary-links')); ?>         
          <?php endif; ?>
          
          <?php if (!empty($main_nav)) print $main_nav; ?>
          
        </div>        
        <!-- /main-nav -->
        <?php endif; ?>
        
      </div>
      <!-- /header -->
      
      <?php if ($accesibility_links):?>
      <a class="accessibility-target" name="after-header"></a>
      <?php endif;?>
      
      <?php if (!empty($content_before)):?>
      <div id="before-content" class="extra-region">
        <?php print $content_before; ?>
      </div>
      <!-- /before-content -->
      <?php endif; ?>
      
      <div id="main" <?php if (empty($content_after)) print 'class="footer-spacer"';?>>
        <?php if ($left): ?>
        <div id="sidebar-first" class="region sidebar-region">
          <?php print $left; ?>
        </div>
         <!-- /sidebar-first -->
        <?php endif; ?>       

        <?php if ($right): ?>
        <div id="sidebar-second" class="region sidebar-region">
          <?php print $right; ?>
        </div>
        <!-- /sidebar-second -->
        <?php endif; ?>        

        <div id="content">
          <?php if ($accesibility_links):?>
          <a class="accessibility-target" name="content-anchor"></a>
          <?php endif;?> 
          
          <?php if (!empty($tabs)): ?>
          <div class="tabs">
            <?php print $tabs; ?>
          </div>
          <!-- /tabs -->
          <?php endif; ?>          

         <?php if ($breadcrumb or $help or $messages): ?>        
         <div id="content-info">
           <?php print $breadcrumb; ?>
           <?php print $messages; ?>
           <?php print $help; ?>                 
         </div>         
         <!-- /content-info -->
         <?php endif; ?>         

         <?php if ($mission): ?>
         <div id="mission">
           <?php print $mission; ?>
         </div>
         <!-- /mission -->
         <?php endif; ?>         

         <?php if (!empty($content_top)):?>
         <div id="content-top" class="region content-region">
           <?php print $content_top; ?>
         </div>
         <!-- /content-top -->
         <?php endif; ?>
         
         <?php if ($title): ?>  
         <div id="content-title">
           <<?php ($is_front) ? print 'h2' : print 'h1'; ?> class="title">
             <?php print $title; ?>
           </<?php ($is_front) ? print 'h2' : print 'h1'; ?>>
         </div>
         <?php endif; ?>
           
         <?php if (!empty($content)):?>
         <div id="content-area" class="region content-region">
           <div id="default-content">
             <?php print $content; ?>
           </div>
         </div>
         <!-- /content-area -->
         <?php endif; ?>         

         <?php if (!empty($content_bottom)):?>
         <div id="content-bottom" class="region content-region">
           <?php print $content_bottom; ?>
         </div>
          <!-- /content-bottom -->
         <?php endif; ?>
         
         <?php if ($feed_icons): ?>
           <div class="feed-icons"><?php print $feed_icons; ?></div>
         <?php endif; ?>
        
        </div>
        <!-- /content -->
      </div>
      <!-- /main -->
     
      <?php if (!empty($content_after)):?>
      <div id="after-content" class="region footer-spacer extra-region">
        <?php print $content_after; ?>
      </div>
       <!-- /after-content -->   
      <?php endif; ?>
      
      <div id="footer">
        <?php if (!empty($footer)):?>        
        <div id="footer-blocks" class="region">
          <?php print $footer; ?>    
        </div>
        <!--/footer-blocks-->
        <?php endif;?>
        
        <?php if (!empty($footer_message)):?>
        <div id="footer-message">
          <?php print $footer_message; ?>    
        </div>
        <!--/footer-message-->
        <?php endif;?>
        
        <?php if ($accesibility_links):?>
        <a class="accessibility-link" href="#top"><?php print t('[Jump to Top]');?></a>
        <a class="accessibility-link" href="#content-anchor"><?php print t('[Jump to Main Content]');?></a>
        <?php endif;?>  
      
      </div>
      <!-- /footer -->      
    
    </div>
    <!-- /page -->  
    
    <?php print $closure; ?>
  
  </body>
</html>