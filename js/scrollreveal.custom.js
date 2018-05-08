/**
* @file
* Javascript, modifications of DOM.
*
* Manipulates links to include scrollreveal data
*/

(function ($) {
  Backdrop.behaviors.scrollreveal = {
    attach: function (context, settings) {
      var timenow = Date.now();
      var triggers = Backdrop.settings.scrollreveal.triggers_fieldset;
      var config_settings = [];
      //Defaults
      config_settings['distance'] = Backdrop.settings.scrollreveal.config.distance + 'px';
      config_settings['duration'] = Backdrop.settings.scrollreveal.config.duration;
      config_settings['delay'] = Backdrop.settings.scrollreveal.config.delay;
      config_settings['rotate'] = { x: Backdrop.settings.scrollreveal.config.rotate_x, y: Backdrop.settings.scrollreveal.config.rotate_y, z: Backdrop.settings.scrollreveal.config.rotate_z };
      config_settings['opacity'] = Backdrop.settings.scrollreveal.config.opacity;
      config_settings['scale'] = Backdrop.settings.scrollreveal.config.scale;
      config_settings['viewFactor'] = parseInt(Backdrop.settings.scrollreveal.config.viewFactor);
      config_settings['interval'] = parseInt(Backdrop.settings.scrollreveal.config.interval);
      window.sr = ScrollReveal( );
      $.each(triggers, function(key, trigger) {
        config_settings['origin'] = trigger.origin;
        if (trigger.move) {
          config_settings['distance'] = trigger.distance + 'px';
        };
        if (trigger.duration) {
          config_settings['duration'] = trigger.duration;
        };
        if (trigger.delay) {
          config_settings['delay'] = trigger.delay;
        };
        if (trigger.rotate_x) {
          config_settings['rotate'] = { x: trigger.rotate_x, y: trigger.rotate_y, z: trigger.rotate_z  };
        };
        if (trigger.opacity) {
          config_settings['opacity'] = trigger.opacity;
        };
        if (trigger.scale) {
          config_settings['scale'] = trigger.scale;
        };
        config_settings['easing'] = trigger.easing;
        config_settings['mobile'] = trigger.mobile;
        config_settings['reset'] = trigger.reset;
        config_settings['useDelay'] = trigger.useDelay;
        if (trigger.viewFactor) {
          config_settings['viewFactor'] = parseInt(trigger.viewFactor);
        };
        if (trigger.interval) {
          config_settings['interval'] = parseInt(trigger.interval);
        };
        var config = {
          origin: config_settings['origin'],
          distance: config_settings['distance'],
          duration: config_settings['duration'],
          delay: config_settings['delay'],
          rotate: config_settings['rotate'],
          opacity: config_settings['opacity'],
          scale: config_settings['scale'],
          easing: config_settings['easing'],
          mobile: config_settings['mobile'],
          reset: config_settings['reset'],
          useDelay: config_settings['useDelay'],
          viewFactor: config_settings['viewFactor'],
        };
        sr.reveal(trigger.element, config, config_settings['interval']);
      });
    }
  }

}(jQuery));
