/**
 * Created by Scott Johnstone on 28/08/14.
 */

(function ($) {

  Drupal.behaviors.addHighlight = {
    attach: function (context, settings) {

      // If radio button is clicked change the background of the fieldset
      $('.radios').click(function () {
        var radioVal = $(this).children('input').attr('value');

        if (radioVal === '1') {
          $(this).parents('.ratings-container').addClass('satisfactory');
          $(this).parents('.ratings-container').removeClass('deficient');
        }
        else if (radioVal === '2') {
          $(this).parents('.ratings-container').addClass('deficient');
          $(this).parents('.ratings-container').removeClass('satisfactory');
        }
        else {
          $(this).parents('.ratings-container').removeClass('satisfactory');
          $(this).parents('.ratings-container').removeClass('deficient');
        }

      }); // End the radio click function

    }
  };

  Drupal.behaviors.addCheckboxValues = {
    attach: function (context, settings) {

      // Set our cookie to accept json data
      $.cookie.json = true;

      // Remove cookie if ajax event occurs because a new category has been selected.
      $(document).ajaxComplete(function (event, xhr, settings) {
        if (settings.url === "/system/ajax") {
          $.removeCookie('checkboxValues');
        }
      });

      function repopulateCheckboxes() {
        var checkboxes = {};
        var num = [];

        checkboxes = $.cookie('checkboxValues');
        console.log(checkboxes);
        // If a cookie exists
        if (checkboxes) {
          // Loop through id's
          Object.keys(checkboxes).forEach(function (element) {
            // Get true or false value for element
            var checked = checkboxes[element];

            if (checked) {
              var id = "#" + element;
              // Add Active class to any element that is TRUE
              $(id).prop("checked", checked).parent().addClass('active');
              // Grab number from end of id
              num = element.match(/\d+$/);
              // If radio was selected
              if (Number(num)) {
                if (num[0] === '1') {
                  $(id).parents('.ratings-container').addClass('satisfactory').removeClass('deficient');
                }
                if (num[0] === '2') {
                  $(id).parents('.ratings-container').addClass('deficient').removeClass('satisfactory');
                }
                if (num[0] === '3') {
                  $(id).parents('.ratings-container').removeClass('satisfactory').removeClass('deficient');
                }
              }
            }
          });

        }
      }


      // Run when any checkbox is clicked
      $("input:checkbox, input:radio", context).on("change", function () {

        var id = $(this).attr('id'); // Grab radio value and send as parameter
        var checkboxValues = {};

        // Grab all the checkboxes checked true or false values
        $("input:checkbox, input:radio", context).each(function () {
          checkboxValues[this.id] = this.checked;
        });
        // Set cookie
        $.cookie('checkboxValues', checkboxValues);

      });

      // Repopulate checkbox and radios only if cookie exists
      if ($.cookie('checkboxValues')) {
        repopulateCheckboxes();
      }


    }
  };

})(jQuery);