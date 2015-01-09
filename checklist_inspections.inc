<?php
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_inspections.class.inc.php');
require_once(DRUPAL_ROOT . '/' . drupal_get_path('module', 'checklist_inspections') . '/includes/checklist_category.class.inc.php');

drupal_add_css(drupal_get_path('module', 'checklist_inspections') . '/dist/css/min/check_inspect_form.min.css', array('scope' => 'footer'));
drupal_add_js(drupal_get_path('module', 'checklist_inspections') . '/dist/js/checklist_inspections.min.js');

/**
 * Show form once callback has fired.
 *
 * @return mixed
 */
function checklist_inspections_page() {
  return drupal_get_form('checklist_inspect_form');
}

/**
 * Implements hook_form().
 * Checklist Inspections Form
 */
function checklist_inspect_form($form, &$form_state) {

  unset($_SESSION['repair_data']); // unset $_SESSION just in case they are submitting again

  drupal_add_library('system', 'ui.datepicker');

  global $user;
  $checklist_obj = new ChecklistInspections(); // Get repair order object.

  $equip_category = $checklist_obj->get_equipment_categories();
  $order_num = $checklist_obj->check_inspect_order_num();
  $form = array();

  // Add some text
  $form['user_message'] = array(
    '#type' => 'markup',
    '#markup' => '<div class="page-description" id="page-description-' . $order_num . '"><p>Admin page for submitting a checklist inspection form for approval.</p></div>',
  );

  $form['container'] = array(
    '#type' => 'fieldset',
  );

  $form['container']['row_1_open']['#prefix'] = '<div class="row">';

  $form['container']['order_num'] = array(
    '#type' => 'textfield',
    '#title' => 'Order #',
    '#value' => $order_num,
    '#disabled' => TRUE,
    '#prefix' => '<div class="order-num" id="order-num-' . $order_num . '">',
    '#suffix' => '</div>',
  );

  $form['container']['date_submitted'] = array(
    '#type' => 'textfield',
    '#title' => 'Date',
    '#value' => date('F d, Y'),
    '#size' => 25,
    '#disabled' => TRUE,
//    '#id' => 'date-submitted',
    '#prefix' => '<div class="date-submitted" id="date-submitted-' . $order_num . '">',
    '#suffix' => '</div>',
  );

  $form['container']['category'] = array(
    '#type' => 'select',
    '#title' => 'Equipment Category',
    '#options' => drupal_map_assoc($equip_category),
    '#required' => TRUE,
    //    '#attach' => drupal_add_js('
    //        (function ($) {
    //          $(document).ready(function () {
    //            $("#edit-category :first-child").text("- Select Category -");
    //          });
    //        })(jQuery)',
    //    'inline'),
    '#ajax' => array(
      'callback' => 'get_unit_id_select_callback',
      'wrapper' => 'unit_id_select_wrapper',
      'method' => 'replace',
      'effect' => 'fade',
    ),
    '#prefix' => '<div class="category" id="category-' . $order_num . '">',
    '#suffix' => '</div>',
  );

  // Category title
  $form['container']['category_wrapper'] = array(
    '#prefix' => '<div id="unit_id_select_wrapper">',
    '#suffix' => '</div>',
  );

  // Get category after user selects category
  $category = isset($form_state['values']['category']) ?
    $form_state['values']['category'] : array();

  // Get the title for the Unit ID
  $form['container']['unit_id_title'] = array(
    '#prefix' => '<div id="unit_id_title_wrapper">',
    '#suffix' => '</div>',
  );

  if ($category) {
    $unit_id_select = $checklist_obj->set_select_options_list($category);
    // Save node id for saving to database
    $form['nid'] = array(
      '#type' => 'value',
      '#value' => $unit_id_select,
    );

//    dsm($unit_id_select);
    $form['container']['category_wrapper']['unit_id_select'] = array(
      '#type' => 'select',
      '#title' => 'Unit ID',
      '#options' => drupal_map_assoc($unit_id_select),
      //      '#attach' => drupal_add_js('
      //        (function ($) {
      //          $(document).ready(function () {
      //            $("#edit-unit-id-select :first-child").text("- Select Unit ID -");
      //          });
      //        })(jQuery)',
      //        'inline'),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'get_unit_id_title_callback',
        'wrapper' => 'unit_id_title_wrapper',
        'method' => 'replace',
        'effect' => 'fade',
      ),
      '#prefix' => '<div class="">',
      '#suffix' => '</div>',
    );
  }

  $form['container']['row_1_closed']['#suffix'] = '</div><div class="row">'; // Close the first row, Open second row

  $form['container']['inspected_by'] = array(
    '#type' => 'textfield',
    '#title' => t('Inspected By'),
    '#value' => $user->name,
    '#disabled' => TRUE,
        '#prefix' => '<div class="inspected-by" id="inspected-by-' . $order_num . '">',
        '#suffix' => '</div>',
  );

  $form['container']['checklist_message']['#markup'] = '<div class="checklist-message" id="checklist-message-' . $order_num . '"><p id="user-instructions"><strong>S</strong> = <em id="satisfactory">Satisfactory</em>&nbsp
        <strong>D</strong> = <em id="deficient">Deficient</em>&nbsp <strong>N/A</strong> = <em id="NA"> Not Applicable</em></p></div>';

  $form['container']['row_2_closed']['#suffix'] = '</div>'; // Close second row


  //-------------- If unit_id_select was chosen then we call our object class --------------------//
  $unit_id = isset($form_state['values']['unit_id_select']) ?
    $form_state['values']['unit_id_select'] : array();

  $form['container']['equip_state'] = array(
    '#type' => 'fieldset',
    '#title' => t('State of Repair'),
    '#id' => t('edit-equip-state'),
    '#attributes' => array(
      'class' => array(''),
    ),
  );


  if ($category && $unit_id) {

    $category_obj = new ChecklistCategories();
    $category_list = $category_obj->build_category_list($category);
    $rows = array();
    $repair_states = array();


    foreach ($category_list as $box => $rad) {

      // Reorganize into the array format we need
      if ($box === 'title') {
        foreach ($rad as $y => $x) {
          $rows[$y] = $x;
        }
      }
      if ($box === 'repair_state') {
        foreach ($rad as $b => $c) {
          $repair_states[$b] = $c;
        }
      }
    }

    // Start building the all the checkboxes inside each fieldset.
    foreach ($rows as $key => $state) {

      $form['container']['equip_state'][$key] = array(
        '#type' => 'fieldset',
        '#id' => 'ratings-container-' . $order_num,
        '#attributes' => array('class' => array('ratings-container')),
        '#tree' => TRUE,
      );

      //Build the checkbox states
      $form['container']['equip_state'][$key][$state] = array(
        '#type' => 'markup',
        '#markup' => t('<div class="status-titles pull-left"><strong>@state</strong></div>', array('@state' => $state)),
      );


      foreach ($repair_states as $key_val => $cond) {
//        dsm($key_val . ': '. $key);
        // Build our radio buttons
        $form['container']['equip_state'][$key][$key . '-' . $cond] = array(
          '#type' => 'markup',
          '#markup' => '<div class="btn-group btn-group-sm pull-right radio-buttons" data-toggle="buttons">
                          <label for="order-' . $order_num . '-'  . $key . '_1" class="btn btn-primary radios">
                            <input type="radio" name="' . $key . '_radio" id="order-' . $order_num . '-' . $key . '_1" value="1">S
                          </label>
                          <label for="order-' . $order_num . '-' . $key . '_2" class="btn btn-primary radios">
                            <input type="radio" name="' . $key . '_radio" id="order-' . $order_num . '-' . $key . '_2" value="2">D
                          </label>
                          <label for="order-' . $order_num . '-' . $key . '_3" class="btn btn-primary radios active">
                            <input type="radio" name="' . $key . '_radio" id="order-' . $order_num . '-' . $key . '_3" value="3" checked="true">N/A
                          </label>
                        </div>'
        );

        // Compare key value in $repairs_state array with key value in $rows titles
        // being cycled in the outside foreach loop.
        if ($key_val == $key) {

          $form['container']['equip_state'][$key]['checkbox_group'] = array(
            '#prefix' => '<div class="btn-group btn-group-sm pull-right damage-buttons" data-toggle="buttons">',
            '#suffix' => '</div>',
          );
          // If there is a match than print checkboxes for each repair state value
          foreach ($cond as $val) {

            $form['container']['equip_state'][$key]['checkbox_group'][$val] = array(
              '#type' => 'markup',
              '#markup' => '<label for="order-' . $order_num . '-' . $val . '" class="btn btn-danger">
                                <input type="checkbox"  name="' . $key . '_' . $val . '" id="order-' . $order_num . '-' . $val . '">' . $val . '</label>'
            );
          }
        }
      }

      $is_true = FALSE;
      switch ($key) {
        case 'wiring':
          $is_true = TRUE;
          break;
        case 'dry_amps':
          $is_true = TRUE;
          break;
        case 'pumping_amps':
          $is_true = TRUE;
          break;
        case 'pressure':
          $is_true = TRUE;
          break;
        case 'propane_level':
          $is_true = TRUE;
          break;
        case 'certification':
          $is_true = TRUE;
          break;
        case 'tire_pressure_tires':
          $is_true = TRUE;
          break;
        case 'engine_hours':
          $is_true = TRUE;
          break;
        case 'oil_level':
          $is_true = TRUE;
          break;
        case 'fuel_level':
          $is_true = TRUE;
          break;
        case 'trans_and_trans_case':
          $is_true = TRUE;
          break;
        case 'differentials':
          $is_true = TRUE;
          break;
        case 'hydraulic_tank':
          $is_true = TRUE;
          break;
        case 'fuel_tank':
          $is_true = TRUE;
          break;
        case 'engine_coolant':
          $is_true = TRUE;
          break;
        case 'fire_extinguisher':
          $is_true = TRUE;
          break;
        case 'window_wipers_washer':
          $is_true = TRUE;
          break;
        default;
      }

      // Only print textfields that apply
      if ($is_true) {
        $form['container']['equip_state'][$key][$key . '_textbox'] = array(
          '#type' => 'textfield',
          '#default_value' => '',
          '#size' => 60,
          '#id' => 'order-' . $order_num . '-text-box',
          '#attributes' => array('class' => array('text-box')),
        );
      }
    }
  }

  $form['container']['additional_notes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Additional Notes'),
  );

  $form['container']['additional_notes']['comments'] = array(
    '#type' => 'textarea',
    '#title' => t('Comments'),
    '#default_value' => '',
    '#id' => 'order-' . $order_num . '-comments',
    '#attributes' => array('class' => array('comments')),
  );

  $form['container']['additional_notes']['row_4_open']['#markup'] = '<div class="row">';

  $form['container']['additional_notes']['follow_up_alert'] = array(
    '#type' => 'select',
    '#title' => t('Follow Up By'),
    '#required' => TRUE,
    '#empty_option' => t('- Select Alert Time -'),
    '#options' => array(
      '14 days' => t('2 Weeks'),
      '30 days' => t('1 month'),
      '60 days' => t('2 months'),
      '90 days' => t('3 months'),
    ),
    '#attributes' => array('class' => array('follow-up-alert')),
    '#id' => 'order-' . $order_num . '-follow-up-alert',
    '#prefix' => '<div class="follow-up-container">',
    '#suffix' => '</div>',
  );

  $form['container']['additional_notes']['submit_button'] = array(
    '#type' => 'submit',
    '#value' => 'Submit Checklist',
    '#attributes' => array(
      'class' => array('btn btn-primary'),
    ),
    '#prefix' => '<div id="submit-button">',
    '#suffix' => '</div>',
  );

  $form['container']['additional_notes']['row_4_closed']['#markup'] = '</div>';


  return $form;
}


/**
 * Validate the checklist_form
 */
function checklist_inspect_form_validate($form, &$form_state) {

  $repair_vals = array();
  $order_num = $form_state['values']['order_num'];

  // Calculate the follow-up-alert
  $date_submitted = $form_state['values']['date_submitted'];
  $alert = $form_state['values']['follow_up_alert'];

  $date = date_create($date_submitted);
  date_add($date, date_interval_create_from_date_string($alert));
  $alert_date = date_format($date, 'F d, Y');
  // Add to $form_state
  $form_state['values']['follow_up_alert'] = $alert_date;


  foreach ($form_state['input'] as $key => $val) {

    // Filter $form_state values to just the values we need
    if (($key === 'submit_button') || ($key === 'date_submitted') || ($key === 'op') || ($key === 'form_build_id') ||
        ($key === 'category') || ($key === 'inspected_by') || ($key === 'comments') || ($key === 'unit_id_select') ||
        ($key === '_triggering_element_name') || ($key === 'ajax_html_ids') || ($key === 'ajax_page_state') ||
        ($key === 'follow_up_alert') || /*($key === 'order_num') ||*/ ($key === 'form_token') || ($key === 'form_id'))
    {
      continue;
    }

    // Build our repair values array to store as a session
    // We need unique id's for every order for future pages,
    // so not to get duplicate id's on pages showing multiple forms.
    if (isset($val) && gettype($val) === 'string') {
      // If it is a radio button
      if (is_numeric($val)) {
        $repair_vals['order-'. $order_num . '-' . $key . '_' .$val] = $val;
      }
      // It is a checkbox button
      else {
        $repair_vals['order-'. $order_num . '-' . $key] = $val;
      }
    }
    // If not a checkbox or radio button, then it's the textbox values
    else if (isset($val) && gettype($val) === 'array') {

      foreach ($val as $a => $b) {
        $repair_vals['order-'. $order_num . '-' . $key] = $b;
      }
    }
  }

  $order = array();
  $order[$order_num] = $repair_vals; // Need the order_num for javascript later on

  // Serialize the data to pass to submit function
  $store_repairs = serialize($order);
//  $test = unserialize($store_repairs);
//  dsm($test);
//  dsm($form_state);
  $form_state['values']['repair_data'] = $store_repairs;
  $form_state['values']['repair_archive'] = $store_repairs;
}

/**
 * Submit the checklist_form
 */
function checklist_inspect_form_submit($form, &$form_state) {

  global $user;
  // Save the nid and all other repair data to the database
  $nid_match = array_search($form_state['values']['unit_id_select'], $form_state['values']['nid']);

  $query = db_insert('checklist_inspections');
  $fields = array(
    'category' => $form_state['values']['category'],
    'order_num' => $form_state['values']['order_num'],
    'unit_id' => $form_state['values']['unit_id_select'],
    'nid' => $nid_match,
//    'submitted' => 'true',
    'inspected_by' => $form_state['values']['inspected_by'],
    'comments' => $form_state['values']['comments'],
    'repair_data' => $form_state['values']['repair_data'],
    'repair_archive' => $form_state['values']['repair_archive'],
    'date_submitted' => $form_state['values']['date_submitted'],
    'follow_up_alert' => $form_state['values']['follow_up_alert'],
    'time_stamp' => time(),
  );

  $query->fields($fields);
  $query->execute();

  // Save session for print page
  $_SESSION['repair_data'] = $fields;

  // Get emails that have been set in the transfer request configuration page.
  $admin_mail = variable_get('transfer_admin_email');
  // Send email to administrator notifying him of request.
  $to = $admin_mail;
  $from = $user->mail;

  $params['order_num'] =  $form_state['values']['order_num'];
  $params['date'] = $form_state['values']['date_submitted'];

  $sent = drupal_mail('checklist_inspections', 'new_checklist', $to, language_default(), $params, $from, TRUE);
  // Show error if mail does not get sent for whatever reason.
  if(!$sent) {
    drupal_set_message(t('An error occurred and admin mail was not sent. Please notify administrator.'), 'error');
  }


  drupal_set_message(t('Thank You! The Checklist Inspection has been submitted.'));

  $form_state['redirect'] = 'checklist_inspections/print';
}

/**
 * Ajax callback for getting and setting the Unit Id for the title
 */
function get_unit_id_title_callback($form, &$form_state) {

  $commands = array();
  $unit_id_title = $form_state['values']['unit_id_select'];
  $commands[] = ajax_command_replace('#unit_id_title_wrapper', drupal_render($form['container']['unit_id_title']));
  $commands[] = ajax_command_remove('#unit_id_title');
  $commands[] = ajax_command_append('#heading', '<h3 id="unit_id_title">' . $unit_id_title . '</h3>');

  // Now add the fields
  $commands[] = ajax_command_replace('#edit-equip-state', drupal_render($form['container']['equip_state']));

  $output = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );

  return $output;
}

/**
 * Ajax callback for updating the unit_id select dropdown list.
 */
function get_unit_id_select_callback($form, &$form_state) {

  $commands = array();
  $heading = $form_state['values']['category'];

  $commands[] = ajax_command_replace('#unit_id_select_wrapper', drupal_render($form['container']['category_wrapper']));
  // This will clear the State of Repair field if user re-choices another Category
  $commands[] = ajax_command_replace('#edit-equip-state',
    '<fieldset class="panel panel-default form-wrapper" id="edit-equip-state"><legend class="panel-heading">
       <div class="panel-title fieldset-legend">State of Repair</div></legend> <div class="panel-body"> </div> </fieldset>'
  );

  $commands[] = ajax_command_remove('#heading');
  $commands[] = ajax_command_prepend('#edit-container', '<h2 id="heading">' . $heading . '</h2>');

  $output = array(
    '#type' => 'ajax',
    '#commands' => $commands,
  );

  return $output;
}