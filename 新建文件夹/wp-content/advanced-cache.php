<?php
defined( 'ABSPATH' ) or die( 'Cheatin\' uh?' );

define( 'WP_ROCKET_ADVANCED_CACHE', true );
$rocket_cache_path = '/www/wwwroot/optobotlab.com/wp-content/cache/wp-rocket/';
$rocket_config_path = '/www/wwwroot/optobotlab.com/wp-content/wp-rocket-config/';

if ( file_exists( '/www/wwwroot/optobotlab.com/wp-content/plugins/wp-rocket/inc/front/process.php' ) ) {
	include( '/www/wwwroot/optobotlab.com/wp-content/plugins/wp-rocket/inc/front/process.php' );
} else {
	define( 'WP_ROCKET_ADVANCED_CACHE_PROBLEM', true );
}