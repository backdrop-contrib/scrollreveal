<?php
/**
 * @file
 * Administration page callbacks for the Scroll Reveal module.
 */

/**
 * Callback function for admin settings form.
 */
function scrollreveal_admin($form, &$form_state) {
  // Since the form builder is called after every AJAX request, we rebuild
  // the form based on $form_state.
  $settings = config_get('scrollreveal.settings', 'scrollreveal_settings');
  $num_elements = isset($settings['triggers_fieldset']) ? count($settings['triggers_fieldset']) + 1 : 1;
  $form_state['triggers'] = $num_elements;
  $themes = list_themes();
  $active_themes = array();
  foreach ($themes as $theme) {
    if ($theme->status) {
      $active_themes[$theme->name] = $theme->info['name'];
    }
  }
  backdrop_add_css(backdrop_get_path('module', 'scrollreveal') . "/css/scrollreveal.css");

  $options['helper'] = array(
    '#markup' => t('<h3>Scroll Reveal Options</h3>
<p>A simple way to create and maintain how elements fade in, triggered when they enter the viewport, using the Scroll Reveal plugin from Julian Lloyd.</p>'),
  );
  $options['config'] = array(
    '#type' => 'fieldset',
    '#title' => t('Default Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options['config']['origin'] = array(
    '#type' => 'select',
    '#title' => "Origin",
    '#default_value' => 'top',
    '#options' => array(
      'top' => t('Top'),
      'bottom' => t('Bottom'),
      'right' => t('Right'),
      'left' => t('Left'),
    ),
    '#default_value' => $settings['config']['origin'],
  );
  $options['config']['distance'] = array(
    '#type' => 'textfield',
    '#title' => "Distance (in pixels)",
    '#default_value' => 0,
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['distance'],
  );
  $options['config']['duration'] = array(
    '#type' => 'textfield',
    '#title' => "Duration (in ms)",
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['duration'],
  );
  $options['config']['delay'] = array(
    '#type' => 'textfield',
    '#title' => "Delay (in ms)",
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['delay'],
  );
  $options['config']['interval'] = array(
    '#type' => 'textfield',
    '#title' => "Sequence interval (in ms)",
    '#description' => 'The interval is the time until the next element in the sequence begins its reveal, which is separate from the time until the element’s animation completes.',
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['interval'],
  );
  $options['config']['rotate_x'] = array(
    '#type' => 'textfield',
    '#title' => "Rotate X",
    '#description' => 'Starting angles in degrees, will transition from these values to 0 in all axes.',
    '#size' => 6,
    '#maxlength' => 3,
    '#default_value' => $settings['config']['rotate_x'],
  );
  $options['config']['rotate_y'] = array(
    '#type' => 'textfield',
    '#title' => "Rotate Y",
    '#size' => 6,
    '#maxlength' => 3,
    '#default_value' => $settings['config']['rotate_y'],
  );
  $options['config']['rotate_z'] = array(
    '#type' => 'textfield',
    '#title' => "Rotate Z",
    '#size' => 6,
    '#maxlength' => 3,
    '#default_value' => $settings['config']['rotate_z'],
  );
  $options['config']['opacity'] = array(
    '#type' => 'textfield',
    '#title' => "Opacity",
    '#description' => 'Starting opacity value, before transitioning to the computed opacity.',
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['opacity'],
  );
  $options['config']['scale'] = array(
    '#type' => 'textfield',
    '#title' => "Scale",
    '#description' => 'Starting scale value, will transition from this value to 1.',
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['scale'],
  );
  $options['config']['easing'] = array(
    '#type' => 'select',
    '#title' => t('Easing effect'),
    '#options' => array(
      'ease' => t('Ease'),
      'ease-in' => t('Ease In'),
      'ease-out' => t('Ease Out'),
      'ease-in-out' => t('Ease In Out'),
    ),
    '#default_value' => $settings['config']['easing'],
  );
  $options['config']['mobile'] = array(
    '#type' => 'select',
    '#title' => t('Mobile'),
    '#description' => t("Control reveal animations on mobile."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#default_value' => $settings['config']['mobile'],
  );
  $options['config']['reset'] = array(
    '#type' => 'select',
    '#title' => t('Reset effect'),
    '#description' => t("Replay reveal animations every time elements enter the viewport."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#default_value' => $settings['config']['reset'],
  );
  $options['config']['useDelay'] = array(
    '#type' => 'select',
    '#title' => t('Use Delay'),
    '#options' => array(
      'always' => t('Always - delay all reveal animations'),
      'once' => t('Once - delay only the first time reveals occur'),
      'onload' => t('Onload - delay only for animations triggered by first load'),
    ),
    '#default_value' => $settings['config']['useDelay'],
  );
  $options['config']['viewFactor'] = array(
    '#type' => 'textfield',
    '#title' => "Viewport",
    '#description' => t("When should an element be considered in the viewport? The default value of 0.20 means 20% of an element must be visible for its reveal to occur."),
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => $settings['config']['viewFactor'],
  );
  $options['triggers_fieldset'] = array(
    '#tree' => TRUE,
    '#title' => t("Target elements"),
    // The prefix/suffix provide the div that we're replacing, named by
    // #ajax['wrapper'] above.
    '#prefix' => '<div id="triggers-div">',
    '#suffix' => '</div>',
    '#type' => 'vertical_tabs',
    '#description' => t('Elements where Scroll Reveal effects will be added'),
  );
  $i = 1;
  $triggers = isset($settings['triggers_fieldset']) ? $settings['triggers_fieldset'] : array();
  foreach ($triggers as $trigger) {
    $options['triggers_fieldset']["trigger$i"] = array(
      '#type' => 'fieldset',
      '#title' => $trigger['element'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#group' => 'triggers_fieldset'
    );
    $options['triggers_fieldset']["trigger$i"]['element'] = array(
      '#type' => 'textfield',
      '#default_value' => isset($trigger['element']) ? $trigger['element'] : '',
      '#title' => "Valid jQuery selector",
      '#required' => TRUE,
      '#description' => "A jQuery selector to identify the element(s) that will appear with the Scroll Reveal. For example for a list of items in a view you could use #block-views-services-block .views-row.",
    );
    $options['triggers_fieldset']["trigger$i"]['origin'] = array(
      '#type' => 'select',
      '#title' => "Origin",
      '#default_value' => isset($trigger['origin']) ? $trigger['origin'] : '',
      '#options' => array(
        'top' => t('Top'),
        'bottom' => t('Bottom'),
        'right' => t('Right'),
        'left' => t('Left'),
      ),
      '#empty_option' => t('- Default -'),
    );
    $options['triggers_fieldset']["trigger$i"]['distance'] = array(
      '#type' => 'textfield',
      '#title' => "Distance (in pixels)",
      '#default_value' => isset($trigger['distance']) ? $trigger['distance'] : '',
      '#size' => 6,
      '#maxlength' => 4,
    );
    $options['triggers_fieldset']["trigger$i"]['duration'] = array(
      '#type' => 'textfield',
      '#title' => "Duration (in ms)",
      '#default_value' => isset($trigger['duration']) ? $trigger['duration'] : '',
      '#size' => 6,
      '#maxlength' => 4,
    );
    $options['triggers_fieldset']["trigger$i"]['delay'] = array(
      '#type' => 'textfield',
      '#title' => "Delay (in ms)",
      '#default_value' => isset($trigger['delay']) ? $trigger['delay'] : '',
      '#size' => 6,
      '#maxlength' => 4,
    );
    $options['triggers_fieldset']["trigger$i"]['interval'] = array(
      '#type' => 'textfield',
      '#title' => "Sequence interval (in ms)",
      '#description' => 'The interval is the time until the next element in the sequence begins its reveal, which is separate from the time until the element’s animation completes.',
      '#size' => 6,
      '#maxlength' => 4,
      '#default_value' => isset($trigger['interval']) ? $trigger['interval'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['rotate_x'] = array(
      '#type' => 'textfield',
      '#title' => "Rotate X",
      '#description' => 'Starting angles in degrees, will transition from these values to 0 in all axes.',
      '#size' => 6,
      '#maxlength' => 26,
      '#default_value' => isset($trigger['rotate_x']) ? $trigger['rotate_x'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['rotate_y'] = array(
      '#type' => 'textfield',
      '#title' => "Rotate Y",
      '#size' => 6,
      '#maxlength' => 26,
      '#default_value' => isset($trigger['rotate_y']) ? $trigger['rotate_y'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['rotate_z'] = array(
      '#type' => 'textfield',
      '#title' => "Rotate Z",
      '#size' => 6,
      '#maxlength' => 26,
      '#default_value' => isset($trigger['rotate_z']) ? $trigger['rotate_z'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['opacity'] = array(
      '#type' => 'textfield',
      '#title' => "Opacity",
      '#description' => 'Starting opacity value, before transitioning to the computed opacity.',
      '#size' => 6,
      '#maxlength' => 4,
      '#default_value' => isset($trigger['opacity']) ? $trigger['opacity'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['scale'] = array(
      '#type' => 'textfield',
      '#title' => "Scale",
      '#description' => 'Starting scale value, will transition from this value to 1.',
      '#size' => 6,
      '#maxlength' => 4,
      '#default_value' => isset($trigger['scale']) ? $trigger['scale'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['easing'] = array(
      '#type' => 'select',
      '#title' => t('Easing'),
      '#options' => array(
        'ease' => t('Ease'),
        'ease-in' => t('Ease In'),
        'ease-out' => t('Ease Out'),
        'ease-in-out' => t('Ease In Out'),
      ),
      '#empty_option' => t('- Default -'),
      '#default_value' => isset($trigger['easing']) ? $trigger['easing'] : '',
    );
  $options['triggers_fieldset']["trigger$i"]['mobile'] = array(
    '#type' => 'select',
    '#title' => t('Mobile'),
    '#description' => t("Control reveal animations on mobile."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#empty_option' => t('- Default -'),
    '#default_value' => isset($trigger['mobile']) ? $trigger['mobile'] : '',
  );
    $options['triggers_fieldset']["trigger$i"]['reset'] = array(
      '#type' => 'select',
      '#title' => t('Reset effect'),
      '#description' => t("Replay reveal animations every time elements enter the viewport."),
      '#options' => array(
        'false' => t('False'),
        'true' => t('True'),
      ),
      '#empty_option' => t('- Default -'),
      '#default_value' => isset($trigger['reset']) ? $trigger['reset'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['useDelay'] = array(
      '#type' => 'select',
      '#title' => t('Use Delay'),
      '#options' => array(
        'always' => t('Always - delay all reveal animations'),
        'once' => t('Once - delay only the first time reveals occur'),
        'onload' => t('Onload - delay only for animations triggered by first load'),
      ),
      '#empty_option' => t('- Default -'),
      '#default_value' => isset($trigger['useDelay']) ? $trigger['useDelay'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['viewFactor'] = array(
      '#type' => 'textfield',
      '#title' => "Viewport",
      '#description' => t("When should an element be considered in the viewport? The default value of 0.20 means 20% of an element must be visible for its reveal to occur."),
      '#size' => 6,
      '#maxlength' => 4,
      '#default_value' => isset($trigger['viewFactor']) ? $trigger['viewFactor'] : '',
    );
    $options['triggers_fieldset']["trigger$i"]['delete'] = array(
      '#type' => 'button',
      '#value' => t('Delete element'),
      '#name' => 'delete-' . $i,
      '#submit' => array('scrollreveal_delete_submit'),
      '#attributes' => array('class' => array('button-danger')),
    );
    $i++;
  }
  $options['triggers_fieldset']["trigger$i"] = array(
    '#type' => 'fieldset',
    '#title' => t('New element'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'triggers_fieldset'
  );
  $options['triggers_fieldset']["trigger$i"]['element'] = array(
    '#type' => 'textfield',
    '#default_value' => '',
    '#title' => "Valid jQuery selector",
    '#description' => "A jQuery selector to identify the element(s) that will appear with the Scroll Reveal. For example for a list of items in a view you could use #block-views-services-block .views-row.",
    '#required' => FALSE,
  );
  $options['triggers_fieldset']["trigger$i"]['origin'] = array(
    '#type' => 'select',
    '#title' => "Origin",
    '#default_value' => '',
    '#options' => array(
      'top' => t('Top'),
      'bottom' => t('Bottom'),
      'right' => t('Right'),
      'left' => t('Left'),
    ),
    '#empty_option' => t('- Default -'),
  );
  $options['triggers_fieldset']["trigger$i"]['distance'] = array(
    '#type' => 'textfield',
    '#title' => "Distance (in pixels)",
    '#default_value' => '',
    '#size' => 6,
    '#maxlength' => 4,
  );
  $options['triggers_fieldset']["trigger$i"]['duration'] = array(
    '#type' => 'textfield',
    '#title' => "Duration (in ms)",
    '#default_value' => '',
    '#size' => 6,
    '#maxlength' => 4,
  );
  $options['triggers_fieldset']["trigger$i"]['delay'] = array(
    '#type' => 'textfield',
    '#title' => "Delay (in ms)",
    '#default_value' => '',
    '#size' => 6,
    '#maxlength' => 4,
  );
  $options['triggers_fieldset']["trigger$i"]['interval'] = array(
    '#type' => 'textfield',
    '#title' => "Sequence interval (in ms)",
    '#description' => 'The interval is the time until the next element in the sequence begins its reveal, which is separate from the time until the element’s animation completes.',
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['rotate_x'] = array(
    '#type' => 'textfield',
    '#title' => "Rotate X",
    '#description' => 'Starting angles in degrees, will transition from these values to 0 in all axes.',
    '#size' => 6,
    '#maxlength' => 3,
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['rotate_y'] = array(
    '#type' => 'textfield',
    '#title' => "Rotate Y",
    '#size' => 6,
    '#maxlength' => 3,
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['rotate_z'] = array(
    '#type' => 'textfield',
    '#title' => "Rotate Z",
    '#size' => 6,
    '#maxlength' => 3,
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['opacity'] = array(
    '#type' => 'textfield',
    '#title' => "Opacity",
    '#description' => 'Starting opacity value, before transitioning to the computed opacity.',
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['scale'] = array(
    '#type' => 'textfield',
    '#title' => "Scale",
    '#description' => 'Starting scale value, will transition from this value to 1.',
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['easing'] = array(
    '#type' => 'select',
    '#title' => t('Easing'),
    '#options' => array(
      'ease' => t('Ease'),
      'ease-in' => t('Ease In'),
      'ease-out' => t('Ease Out'),
      'ease-in-out' => t('Ease In Out'),
    ),
    '#empty_option' => t('- Default -'),
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['mobile'] = array(
    '#type' => 'select',
    '#title' => t('Mobile'),
    '#description' => t("Control reveal animations on mobile."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#empty_option' => t('- Default -'),
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['reset'] = array(
    '#type' => 'select',
    '#title' => t('Reset effect'),
    '#description' => t("Replay reveal animations every time elements enter the viewport."),
    '#options' => array(
      'false' => t('False'),
      'true' => t('True'),
    ),
    '#empty_option' => t('- Default -'),
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['useDelay'] = array(
    '#type' => 'select',
    '#title' => t('Use Delay'),
    '#options' => array(
      'always' => t('Always - delay all reveal animations'),
      'once' => t('Once - delay only the first time reveals occur'),
      'onload' => t('Onload - delay only for animations triggered by first load'),
    ),
    '#empty_option' => t('- Default -'),
    '#default_value' => '',
  );
  $options['triggers_fieldset']["trigger$i"]['viewFactor'] = array(
    '#type' => 'textfield',
    '#title' => "Viewport",
    '#description' => t("When should an element be considered in the viewport? The default value of 0.20 means 20% of an element must be visible for its reveal to occur."),
    '#size' => 6,
    '#maxlength' => 4,
    '#default_value' => '',
  );
  $options['pages'] = array(
    '#type' => 'fieldset',
    '#title' => t('Pages Visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options['pages']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Activate on specific pages'),
    '#options' => array(
      0 => t('All pages except those listed'),
      1 => t('Only the listed pages'),
    ),
    '#default_value' => $settings['pages']['visibility'],
  );
  $options['pages']['pages'] = array(
    '#type' => 'textarea',
    '#title' => 'List of pages to activate',
    '#default_value' => $settings['pages']['pages'],
    '#description' => t("Specify pages by using their paths. Enter one path per line. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );
  // Per-path visibility.
  $options['theme'] = array(
    '#type' => 'fieldset',
    '#title' => t('Themes Visibility'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options['theme']['visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Activate on specific themes'),
    '#options' => array(
      0 => t('All themes except those listed'),
      1 => t('Only the listed themes'),
    ),
    '#default_value' => $settings['theme']['visibility'],
  );
  $options['theme']['themes'] = array(
    '#type' => 'select',
    '#title' => 'List of themes where library will be loaded.',
    '#options' => $active_themes,
    '#multiple' => TRUE,
    '#default_value' => $settings['theme']['themes'],
    '#description' => t("Specify in which themes you wish the library to load."),
  );

  $options['#tree'] = TRUE;
  $form['scrollreveal_settings'] = $options;
  $form['actions']['#type'] = 'actions';
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['#submit'][] = 'scrollreveal_admin_form_submit';
  // Disable automatic defaults, which don't work with nested values.
  return $form;
}

/**
 * Now we add a handler/function to validate the data entered into the
 * "year of birth" field to make sure it's between the values of 1900
 * and 2000. If not, it displays an error. The value report is
 * $form_state['values'] (see http://drupal.org/node/144132#form-state).
 *
 * Notice the name of the function. It is simply the name of the form
 * followed by '_validate'. This is always the name of the default validation
 * function. An alternate list of validation functions could have been provided
 * in $form['#validate'].
 *
 * @see scrollreveal_admin()
 */
function scrollreveal_admin_validate($form, &$form_state) {
  if ($form_state['clicked_button']['#value'] == "Delete element") {
    $settings = config_get('scrollreveal.settings', 'scrollreveal_settings');
    $child = $form_state['clicked_button']['#parents'][2];
    unset($settings['triggers_fieldset'][$child]);
    config_set('scrollreveal.settings', 'scrollreveal_settings', $settings);
    $form_state['rebuild'] = TRUE;
    $form_state['flag'] = 1;
    backdrop_set_message(t('The element has been deleted.'));
    backdrop_goto('admin/config/user-interface/scrollreveal');
  }
}

function scrollreveal_admin_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  form_state_values_clean($form_state);
  $i = $form_state['triggers'];
  if (!strlen($form_state['values']['scrollreveal_settings']['triggers_fieldset']["trigger$i"]['element'])) {
    unset($form_state['values']['scrollreveal_settings']['triggers_fieldset']["trigger$i"]);
  }
  unset($form_state['values']['scrollreveal_settings']['triggers_fieldset']['scrollreveal_settings__triggers_fieldset__active_tab']);

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }
    config_set('scrollreveal.settings', $key, $value);
  }

  backdrop_set_message(t('The configuration options have been saved.'));
}
