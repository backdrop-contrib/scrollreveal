<?php
/**
 * @file
 * Administration page callbacks for the Scroll Reveal module.
 */

/**
 * Callback function for admin settings form.
 */
function scrollreveal_admin($form, &$form_state) {
  $settings = config_get('scrollreveal.settings');
  $i = 1;

  // Get active themes.
  $active_themes = array();
  foreach (list_themes() as $theme) {
    if ($theme->status) {
      $active_themes[$theme->name] = $theme->info['name'];
    }
  }

  // Add custom CSS to form.
  backdrop_add_css(backdrop_get_path('module', 'scrollreveal') . "/css/scrollreveal.css");

  // Add default value fields.
  $form['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  scrollreveal_add_form_elements($form['config'], $settings['config']);

  $form['triggers_fieldset'] = array(
    '#type' => 'vertical_tabs',
  );

  // Add element value fields.
  if (!empty($settings['triggers_fieldset'])) {
    foreach ($settings['triggers_fieldset'] as $trigger) {
      $form["trigger$i"] = array(
        '#type' => 'fieldset',
        '#title' => $trigger['element'],
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#group' => 'triggers_fieldset',
      );
      $form["trigger$i"]['element'] = array(
        '#type' => 'textfield',
        '#title' => t('jQuery selector'),
        '#description' => t("A jQuery selector to identify the element(s) that will appear with the Scroll Reveal. For example, for a list of items in a view you could use '.view .views-row'."),
        '#default_value' => isset($trigger['element']) ? $trigger['element'] : '',
        '#required' => TRUE,
      );
      scrollreveal_add_form_elements($form["trigger$i"], $trigger, TRUE, t('- Default -'));
      $form["trigger$i"]['delete'] = array(
        '#type' => 'submit',
        '#value' => t('Delete element'),
        '#submit' => array('scrollreveal_admin_delete_submit'),
        '#limit_validation_errors' => array(),
        '#attributes' => array('class' => array('button-danger')),
      );
      $i++;
    }
  }

  // Add new value fields.
  $form["trigger$i"] = array(
    '#type' => 'fieldset',
    '#title' => t('New element'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'triggers_fieldset'
  );
  $form["trigger$i"]['element'] = array(
    '#type' => 'textfield',
    '#title' => t('jQuery selector'),
    '#description' => t("A jQuery selector to identify the element(s) that will appear with the Scroll Reveal. For example, for a list of items in a view you could use '.view .views-row'."),
    '#default_value' => '',
  );
  scrollreveal_add_form_elements($form["trigger$i"], '', FALSE, t('- Default -'));

  // Add page fields.
  $form['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['pages']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Activate on specific pages'),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
    '#default_value' => $settings['pages']['visibility'],
  );
  $form['pages']['pages'] = array(
    '#type' => 'textarea',
    '#title' => t('List of pages to activate'),
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
    '#default_value' => $settings['pages']['pages'],
  );

  // Add theme fields.
  $form['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Themes visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['theme']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Activate on specific themes'),
    '#options' => array(
      0 => t('All themes except those listed'),
      1 => t('Only the listed themes'),
    ),
    '#default_value' => $settings['theme']['visibility'],
  );
  $form['theme']['themes'] = array(
    '#type' => 'select',
    '#title' => t('List of themes to activate'),
    '#description' => t("Specify in which themes you wish the library to load."),
    '#options' => $active_themes,
    '#default_value' => $settings['theme']['themes'],
    '#multiple' => TRUE,
  );

  $form['#tree'] = TRUE;
  $form['actions'] = array(
    '#type' => 'actions',
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
    ),
  );
  $form['#submit'][] = 'scrollreveal_admin_form_submit';

  return $form;
}

/**
 * Delete an element.
 */
function scrollreveal_admin_delete_submit($form, &$form_state) {
  $element = $form_state['clicked_button']['#parents'][0];
  config_clear('scrollreveal.settings', "triggers_fieldset.$element");
  backdrop_set_message(t('The element has been deleted.'));
}

/**
 * Form submission handler.
 */
function scrollreveal_admin_form_submit($form, &$form_state) {
  $values = &$form_state['values'];
  $config = config('scrollreveal.settings');

  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  unset($values['triggers_fieldset']);
  unset($values['actions']);

  // Save to config.
  foreach ($values as $key => $value) {
    if (substr($key, 0, 7) == 'trigger') {
      // Skip empty elements.
      if (empty($value['element'])) {
        continue;
      }

      $config->set("triggers_fieldset.$key", $value);
    }
    else {
      $config->set($key, $value);
    }
  }
  $config->save();

  backdrop_set_message(t('The configuration options have been saved.'));
}

/**
 * Add elements to a form.
 *
 * Defining the fields once here helps reduce duplication.
 */
function scrollreveal_add_form_elements(&$form, $defaults = '', $isset = FALSE, $empty_option = FALSE) {
  $options['origin'] = array(
    '#type' => 'select',
    '#title' => t('Origin'),
    '#options' => array(
      'top' => t('Top'),
      'bottom' => t('Bottom'),
      'left' => t('Left'),
      'right' => t('Right'),
    ),
  );
  $options['distance'] = array(
    '#type' => 'number',
    '#title' => t('Distance'),
    '#min' => 0,
    '#max' => 9999,
    '#field_suffix' => t('px'),
  );
  $options['duration'] = array(
    '#type' => 'number',
    '#title' => t('Duration'),
    '#min' => 0,
    '#max' => 99999,
    '#field_suffix' => t('ms'),
  );
  $options['delay'] = array(
    '#type' => 'number',
    '#title' => t('Delay'),
    '#min' => 0,
    '#max' => 99999,
    '#field_suffix' => t('ms'),
  );
  $options['interval'] = array(
    '#type' => 'number',
    '#title' => t('Sequence interval'),
    '#description' => t("The interval is the time until the next element in the sequence begins its reveal, which is separate from the time until the element's animation completes."),
    '#min' => 0,
    '#max' => 99999,
    '#field_suffix' => t('ms'),
  );
  $options['rotate_x'] = array(
    '#type' => 'number',
    '#title' => t('Rotate x'),
    '#description' => t("Starting angle; will transition from this value to 0 on the x axis."),
    '#min' => -360,
    '#max' => 360,
    '#field_suffix' => t('degrees'),
  );
  $options['rotate_y'] = array(
    '#type' => 'number',
    '#title' => t('Rotate y'),
    '#description' => t("Starting angle; will transition from this value to 0 on the y axis."),
    '#min' => -360,
    '#max' => 360,
    '#field_suffix' => t('degrees'),
  );
  $options['rotate_z'] = array(
    '#type' => 'number',
    '#title' => t('Rotate z'),
    '#description' => t("Starting angle; will transition from this value to 0 on the z axis."),
    '#min' => -360,
    '#max' => 360,
    '#field_suffix' => t('degrees'),
  );
  $options['opacity'] = array(
    '#type' => 'number',
    '#title' => t('Opacity'),
    '#description' => t("Starting opacity, before transitioning to the computed opacity."),
    '#min' => 0,
    '#max' => 1,
    '#step' => 0.01,
  );
  $options['scale'] = array(
    '#type' => 'number',
    '#title' => t('Scale'),
    '#description' => t("Starting scale value; will transition from this value to 1."),
    '#min' => 0.01,
    '#max' => 99.99,
    '#step' => 0.01,
  );
  $options['easing'] = array(
    '#type' => 'select',
    '#title' => t('Easing effect'),
    '#options' => array(
      'ease' => t('Ease'),
      'ease-in' => t('Ease In'),
      'ease-out' => t('Ease Out'),
      'ease-in-out' => t('Ease In Out'),
    ),
  );
  $options['mobile'] = array(
    '#type' => 'select',
    '#title' => t('Mobile'),
    '#description' => t("Control reveal animations on mobile."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
  );
  $options['reset'] = array(
    '#type' => 'select',
    '#title' => t('Reset effect'),
    '#description' => t("Replay reveal animations every time elements enter the viewport."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
  );
  $options['useDelay'] = array(
    '#type' => 'select',
    '#title' => t('Use delay'),
    '#options' => array(
      'always' => t('Always - delay all reveal animations'),
      'once' => t('Once - delay only the first time reveals occur'),
      'onload' => t('Onload - delay only for animations triggered by first load'),
    ),
  );
  $options['viewFactor'] = array(
    '#type' => 'number',
    '#title' => t('Viewport'),
    '#description' => t("When should an element be considered in the viewport? A value of 0.20 for example means 20% of an element must be visible for its reveal to occur."),
    '#min' => 0,
    '#max' => 1,
    '#step' => 0.01,
  );

  foreach ($options as $key => $value) {
    // Set default values.
    if (empty($defaults)) {
      $options[$key]['#default_value'] = '';
    }
    else {
      if (!$isset) {
        $options[$key]['#default_value'] = $defaults[$key];
      }
      else {
        $options[$key]['#default_value'] = isset($defaults[$key]) ? $defaults[$key] : '';
      }
    }

    // Set empty options.
    if (!empty($empty_option) && $value['#type'] == 'select') {
      $options[$key]['#empty_option'] = $empty_option;
    }

    // Set textfield size.
    if ($value['#type'] == 'textfield') {
      $options[$key]['#size'] = 6;
    }
  }

  $form = array_merge($form, $options);
}

