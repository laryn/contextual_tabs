<?php

/**
 *  ContextualTabs extends Block
 *
 *  This class allows us to create contextual tab blocks.
 */
class ContextualTabs extends Block {
  /**
   * {@inheritdoc}
   */
  function __construct($plugin_name, array $data) {
    parent::__construct($plugin_name, $data);

    $this->settings += array(
      'content' => '',
      'format' => NULL,
    );
  }
  
  /**
   *  Sets title text on draggable block panel in Layout builder.
   */
  function getAdminTitle() {
    return (isset($this->settings['title']) && $this->settings['title'] != '') ? check_plain($this->settings['title']) : t('Contextual tab buttons');
  }

  /**
   *  Sets block subject on block view.
   */
  function getTitle() {
    return isset($this->settings['title']) ? check_plain($this->settings['title']) : '';
  }

  /**
   * Returns the rendered content of this block.
   *
   * @return string
   */
  function getContent() {
    $this->settings['btn_class'] = !empty($this->settings['btn_class']) ? check_plain($this->settings['btn_class']) : 'btn-small btn-sm btn-default';
    $this->settings['content'] = contextual_tabs_render($this->settings);
    return $this->settings['content'];
  }

  /**
   * Returns a preview for this block.
   */
  function getAdminPreview() {
    return t('Displays admin tabs as a contextual menu.');
  }

  /**
   *  Builds the block's configuration form.
   */
  function form(&$form, &$form_state) {
    parent::form($form, $form_state);

    $conf = $this->settings;
    $form['hide_captions'] = array(
      '#type' => 'checkbox',
      '#title' => t('Hide button captions'),
      '#default_value' => (isset($conf['hide_captions'])) ? $conf['hide_captions'] : FALSE,
    );
    $form['direction'] = array(
      '#type' => 'select',
      '#title' => t('Direction'),
      '#options' => array(
        '' => t('Drop down'),
        'up' => t('Drop up'),
      ),
      '#default_value' => (isset($conf['direction'])) ? $conf['direction'] : '',
      '#description' => t('Controls whether menu expands downwards or upwards.  Default is drop down.'),
    );
    $form['alignment'] = array(
      '#type' => 'select',
      '#title' => t('Alignment'),
      '#options' => array(
        '' => t('Left'),
        'right' => t('Right'),
      ),
      '#default_value' => (isset($conf['alignment'])) ? $conf['alignment'] : '',
      '#description' => t('Controls the text alignment of the items in the dropdown menu.'),
    );
    $form['menu_caption'] = array(
      '#type' => 'textfield',
      '#title' => t('Menu caption'),
      '#description' => t('Title/Caption of dropdown menu.  Default is "Configure"'),
      '#default_value' => !empty($conf['menu_caption']) ? $conf['menu_caption'] : t('Configure'),
    );
    $form['icon_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Icon class'),
      '#description' => t('CSS class name for icon.  Default is icon-cog.'),
      '#default_value' => !empty($conf['icon_class']) ? $conf['icon_class'] : 'icon-cog',
    );
    $form['btn_class'] = array(
      '#type' => 'textfield',
      '#title' => t('Button classes'),
      '#description' => t('CSS classes to add to buttons.  Leave blank for default small buttons.'),
      '#default_value' => !empty($conf['btn_class']) ? $conf['btn_class'] : '',
    );
    $form['prevent_alter'] = array(
      '#type' => 'checkbox',
      '#title' => t('Prevent buttons added from other modules'),
      '#default_value' => (isset($conf['prevent_alter'])) ? $conf['prevent_alter'] : FALSE,
    );

    // Hide the title options, as contextual tab blocks don't have titles.
    $form['title_display']['title_display']['#access'] = FALSE;
    $form['title_display']['title']['#access'] = FALSE;

    $form['convert']['label'] = array(
        '#type' => 'textfield',
        '#title' => t('Admin label'),
        '#maxlength' => 64,
        '#description' => t('Used in administrative interfaces and not shown to end-users.'),
        '#weight' => 2,
        '#states' => array(
          'visible' => array(
            ':input[name="reusable"]' => array('checked' => TRUE),
          ),
        ),
      );
      $form['convert']['description'] = array(
       '#type' => 'textfield',
       '#title' => t('Admin description'),
       '#maxlength' => 128,
       '#description' => t('Used in administrative interfaces and not shown to end-users.'),
       '#weight' => 4,
        '#states' => array(
          'visible' => array(
            ':input[name="reusable"]' => array('checked' => TRUE),
          ),
        ),
      );
      $form['convert']['delta'] = array(
        '#type' => 'machine_name',
        '#title' => t('Internal name'),
        '#maxlength' => 64,
        '#machine_name' => array(
          'source' => array('convert', 'label'),
          'exists' => 'block_custom_block_load',
        ),
        '#description' => t('A unique machine-readable name containing letters, numbers, and underscores.'),
        '#weight' => 3,
        '#required' => FALSE,
      );
    }
  

  /**
   * Validates the form settings.
   */
  function formValidate($form, &$form_state) {
    parent::formValidate($form, $form_state);

    if (module_exists('block')) {
      $label = trim($form_state['values']['label']);
      $delta = trim($form_state['values']['delta']);
      if ($form_state['values']['reusable']) {
        if (empty($label)) {
          form_error($form['convert']['label'], t('Admin label is required when making a block reusable.'));
        }
        elseif (empty($delta)) {
          form_error($form['convert']['delta'], t('An internal name is required when making a block reusable.'));
        }
      }
    }
  }

  /**
   * Submit handler to save the form settings.
   */
  function formSubmit($form, &$form_state) {
    parent::formSubmit($form, $form_state);

    $this->settings['info'] = $form_state['values']['label'];
    $this->settings['title'] = $form_state['values']['title'];
    $this->settings['description'] = $form_state['values']['description'];
    $this->settings['body'] = $form_state['values']['content'];
    $this->settings['hide_captions'] = $form_state['values']['hide_captions'];
    $this->settings['direction'] = $form_state['values']['direction'];
    $this->settings['alignment'] = $form_state['values']['alignment'];
    $this->settings['menu_caption'] = $form_state['values']['menu_caption'];
    $this->settings['icon_class'] = $form_state['values']['icon_class'];
    $this->settings['btn_class'] = $form_state['values']['btn_class'];
    $this->settings['prevent_alter'] = $form_state['values']['prevent_alter'];
  }
}
