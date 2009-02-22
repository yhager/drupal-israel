<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
	<head>
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
		<title><?php print $head_title; ?></title>
		<?php print $head; ?>
		<?php print $styles; ?>
	 	<?php print $scripts; ?>
		
		<!--[if lt IE 7]>
			<link rel="stylesheet" href="ie6.css" type="text/css" media="screen" charset="utf-8" />
			<?php if (module_invoke('i18n', 'language_rtl')) :?>
				<link rel="stylesheet" href="ie6_rtl.css" type="text/css" media="screen" charset="utf-8" />
			<?php endif ?>
		<![endif]-->
		
	</head>
	<body class="<?php print $body_classes; ?>">
		<div id="page">
			<div id="header">
				
					<?php if ($logo): ?>
						<div id="site-logo">
							<a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home"><img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" id="logo-image" /></a>
						</div>
					<?php endif; ?><!-- /logo -->
				<div id="site-details">	
					<?php if ($site_name): ?>
						<h1 id='site-name'>
		              		<a href="<?php print $base_path; ?>" title="<?php print t('Home'); ?>" rel="home">
								<?php print $site_name; ?>
							</a>
						</h1>
					<?php endif; ?><!-- /site-name -->
					
					<?php if ($site_slogan): ?>
						<blockquote id='site-slogan'><?php print $site_slogan; ?></blockquote>
					<?php endif; ?><!-- /site-slogan -->
				</div><!-- /site-details -->
				<?php if ($header): ?>
					<div id="header-blocks">
						<?php print $header; ?>
					</div> 
				<?php endif; ?> <!-- /header-blocks -->
			
				<?php if ($main_nav): ?>
					<div id="main-nav">
						<?php print $main_nav; ?>
					</div>
				<?php endif; ?><!-- /main-nav -->
			</div><!-- /header -->
			
			<div id="main">
				
				<?php if ($sidebar_right): ?>
					<div id="sidebar-right">
						<?php print $sidebar_right; ?>
					</div> 
      			<?php endif; ?><!-- /sidebar-right -->
				<?php if ($sidebar_left): ?>
					<div id="sidebar-left">
						<?php print $sidebar_left; ?>
					</div> 
      			<?php endif; ?><!-- /sidebar-left -->
				
				<div id="content">
					<?php if (!empty($tabs)): ?>
						<div class="tabs">
							<?php print $tabs; ?>
						</div>
					<?php endif; ?><!-- /tabs -->
					
					<?php if ($breadcrumb or $title or $help or $messages): ?>
						<div id="content-header">
							<?php print $breadcrumb; ?>	
							<?php print $messages; ?>
				            <?php print $help; ?>
							<?php if ($title): ?>
								<div class="content-title">
									<h2 class="title"><?php print $title; ?></h2>
								</div>
							<?php endif; ?>
 						</div> 
					<?php endif; ?><!-- /content-header -->
					
					<?php if ($mission): ?>
						<div id="mission"><?php print $mission; ?></div>
					<?php endif; ?><!-- /mission -->
					
					<?php if (!empty($content_top)):?>
						<div id="content-top">
							<?php print $content_top; ?>
						</div>
					<?php endif; ?><!-- /content-top -->
					
					<?php if (!empty($content)):?>
						<div id="content-area">
							<?php print $content; ?>
						</div>
					<?php endif; ?><!-- /content -->
					
					<?php if (!empty($content_bottom)):?>
						<div id="content-bottom">
							<?php print $content_bottom; ?>
						</div>
					<?php endif; ?><!-- /content-bottom -->
				</div>	
				
			</div><!-- /main -->
			
			<div id="footer">
				<?php print $footer_message; ?>
			</div><!-- /footer -->
			
			<?php if ($closure_region): ?>
				<div id="closure-blocks"><?php print $closure_region; ?></div>
		    <?php endif; ?>
	
			<?php print $closure; ?>
		</div><!-- /page -->
		 
	</body>

