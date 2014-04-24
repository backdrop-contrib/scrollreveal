/**
* @file
* Javascript, modifications of DOM.
*
* Manipulates links to include scrollreveal data
*/

(function ($) {
  Drupal.behaviors.scrollreveal = {
    attach: function (context, settings) {
      var triggers = Drupal.settings.scrollreveal.triggers_fieldset;
      $.each(triggers, function(key, trigger) {
        var class_string = '';
        if (trigger.enter != "0") {
          class_string += 'enter ' + trigger.enter + ' ';
        };
        if (trigger.move != "0") {
          class_string += 'move ' + trigger.move + 'px ';
        };
        if (trigger.over != "0") {
          class_string += 'over ' + trigger.over + 's ';
        };
        if (trigger.after != "0") {
          class_string += 'after ' + trigger.after + 's ';
        }
        else if (trigger.wait != "0") {
          class_string += 'wait ' + trigger.wait + 's ';
        };
        $(trigger.element).attr( "data-scroll-reveal", class_string.trim() );
      });
      window.scrollReveal = new scrollReveal();
    }
  }  

}(jQuery));
