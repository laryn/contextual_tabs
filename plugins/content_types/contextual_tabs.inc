<?php

/**
 * @file
 * Plugin to handle the 'contextual_tabs' pane
 */

$plugin = array(
  'title' => t('Contextual tab buttons'),
  'single' => TRUE,
  'icon' => 'icon_page.png',
  'description' => t('Displays the contextual tab buttons within a panel pane.'),
  'category' => t('Page elements'),
  'render last' => TRUE,
);

/**
 * Output function for the 'contextual_tabs' content type.
 */
function contextual_tabs_contextual_tabs_content_type_render($subtype, $conf, $panel_messagesrgs) {
  $block = new stdClass();
  $block->content = contextual_tabs_render();
  return $block;
}

function contextual_tabs_contextual_tabs_content_type_admin_info($subtype, $conf) {
  $block = new stdClass;
  $block->title = t('Contextual tab buttons');
  $block->content = t('Displays the contextual tab buttons within a panel pane.');
  return $block;
}
