<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
  <head>
    <?php // $id$ ?>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <?php print $scripts; ?>
    
  </head>
  <body class="<?php print $body_classes; ?>">
    <div id="page">
      <div id="header">
        <div id="header-content">
          <?php if ($logo): ?>
          <<?php ($is_front && !$site-name) ? print 'h1' : print 'div'; ?> id="site-logo">
            <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a>
          </<?php ($is_front && !$site-name) ? print 'h1' : print 'div'; ?>>
          <?php endif; ?>
          <!-- /logo -->
    
          <div id="site-details">
            <?php if ($site_name): ?>
            <<?php ($is_front) ? print 'h1' : print 'div'; ?> id='site-name'>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home">
                <?php print $site_name; ?>
              </a>
            </<?php ($is_front) ? print 'h1' : print 'div'; ?>>
            <?php endif; ?>
            <!-- /site-name -->
            <?php if ($site_slogan): ?>
            <blockquote id='site-slogan'>
              <?php print $site_slogan; ?>
            </blockquote>
            <?php endif; ?>
            <!-- /site-slogan -->
           
          </div>
          <!-- /site-details -->
            
          <div id="header-blocks">            
            
            <?php if (!empty($language_switcher)): ?>
              <div id="language-switcher"><?php print $language_switcher; ?></div>
            <?php endif; ?>
            
            <?php if (!empty($search_box)): ?>
              <div id="search-box"><?php print $search_box; ?></div>
            <?php endif; ?>

            <?php if ($header) print $header; ?>
          
          </div>          
          <!-- /header-blocks -->             
          
          <?php if ($mission): ?>
            <div id="mission">
              <?php print $mission; ?>
            </div>
          <?php endif; ?>
          <!-- /mission -->
          
      </div>
      <!-- header-content -->
        
         
       <div id="main-nav">
          <?php if (!empty($primary_links)): ?>         
            <?php print theme('links', $primary_links, array('class' => 'menu primary-links')); ?>         
          <?php endif; ?>
        </div>
        <!-- /main-nav -->
        
      </div>
      <!-- /header -->
      <?php if (!empty($content_before)):?>
         <div id="before-content" class="extra-region">
           <?php print $content_before; ?>
         </div>
      <?php endif; ?>
      <div id="main" <?php if (empty($content_after)) print 'class="footer-spacer"';?>>
        <?php if ($left): ?>
        <div id="sidebar-first" class="sidebar-region">
          <?php print $left; ?>
        </div>
        <?php endif; ?>
        <!-- /sidebar-left -->

        <?php if ($right): ?>
        <div id="sidebar-second" class="sidebar-region">
          <?php print $right; ?>
        </div>
        <?php endif; ?>
        <!-- /sidebar-right -->

        <div id="content">
          <?php if (!empty($tabs)): ?>
          <div class="tabs">
            <?php print $tabs; ?>
          </div>
          <?php endif; ?>
          <!-- /tabs -->

         <?php if ($breadcrumb or $title or $help or $messages): ?>
         <div id="content-header">  
           <div id="content-info">
             <?php print $breadcrumb; ?>
             <?php print $messages; ?>
             <?php print $help; ?>  
           </div>
           <?php if ($title): ?>  
             <div id="content-title">
               <<?php ($is_front) ? print 'h2' : print 'h1'; ?> class="title">
                 <?php print $title; ?>
               </<?php ($is_front) ? print 'h2' : print 'h1'; ?>>
             </div>
           <?php endif; ?>
         </div>
         <?php endif; ?>
         <!-- /content-header -->

         <?php if (!empty($content_top)):?>
         <div id="content-top" class="content-region">
           <?php print $content_top; ?>
         </div>
         <?php endif; ?>
         <!-- /content-top -->

         <?php if (!empty($content)):?>
         <div id="content-area" class="content-region">
           <div id="default-content">
             <?php print $content; ?>
           </div>
         </div>
         <?php endif; ?>
         <!-- /content -->

         <?php if (!empty($content_bottom)):?>
         <div id="content-bottom" class="content-region">
           <?php print $content_bottom; ?>
         </div>
         <?php endif; ?>
         <!-- /content-bottom -->
        </div>
      </div>
      <!-- /main -->
      <?php if (!empty($content_after)):?>
         <div id="after-content" class="footer-spacer extra-region">
           <?php print $content_after; ?>
         </div>
      <?php endif; ?>
      <div id="footer">
        <?php if (!empty($footer)):?>
          <div id="footer-blocks">
            <?php print $footer; ?>    
          </div>
        <?php endif;?>
        <?php if (!empty($footer_message)):?>
          <div id="footer-message">
            <?php print $footer_message; ?>    
          </div>
        <?php endif;?>
      </div>
      <!-- /footer -->      
    </div>
    <!-- /page -->
    <?php print $closure; ?>
  </body>
</html>
