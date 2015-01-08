(function ($) {
  Drupal.behaviors.addCircleAroundText = {
    attach: function (context, settings) {

      // Get repair status array passed form checklist_submit_print page.
      var circleId = Drupal.settings.checklist_inspections;
      console.log(circleId);

      if(typeof circleId !== 'undefined') {

        $.each(circleId, function(index, value){
          $.each(value, function(key, val) {

              console.log(val);

              var id = '#' + key;
              if (Number(val) === 1) {
                $(id).parent().css({
                  'border': '1px solid green',
                  'border-radius': '20px'
                });
              } else if (Number(val)  === 2) {
                $(id).parent().css({
                  'border': '1px solid red',
                  'border-radius': '20px'
                });
              } else if (Number(val)  === 3) {
                $(id).parent().css({
                  'border': '1px solid #000000',
                  'border-radius': '20px'
                });
              } else {
                $(id).parent().css({
                  'border': '1px solid #000000',
                  'border-radius': '20px'
                });
              }
          });
        });
      }
    }
  };

  Drupal.behaviors.checklist_print = {
    attach: function (context, settings) {

      // When user clicks Print on the print
      // pages we open the a printer window
      $("#print-this-page").click(function() {

        $('div#marble-top').css({
          'box-shadow': 'none'
        });

        $('div.alert-block').remove();

        $('.container').css({
          'margin-top': '0px'
      });
        // Now wait for css to change on page before loading sending to printer
        setTimeout(function(){
          // Now we can print page
          window.print();
        }, 400);

      });

    }
  };
})(jQuery);