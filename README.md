# Before we begin...

Coffee! That's how I get things done!! If you'd like to see me get more things done, please kindly consider <a href="https://www.patreon.com/bePatron?u=8549187" data-patreon-widget-type="become-patron-button">buying me a coffee</a> or two ;)

<p align="center">
 <a href= "https://paypal.me/StoneyDSPAudio?country.x=ES&locale.x=en_US"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif"/></a>
</p>

<p align="center">
 <a href= "https://twitter.com/Stoney_DSP/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/a075caeedffe23b2733ee38b12f9800f62aab9c2/Assets/twitter.png"/></a>
 <a href= "https://www.instagram.com/stoney_dsp/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/2253d684ba99e6c072353a94b49315162c381406/Assets/instagram.png"/></a>
 <a href= "https://www.facebook.com/StoneyDSP.Audio/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/9608562b09ee2708affd0c31117fc25a235672d9/Assets/facebook.png"/></a>
</p>

# Manual - v1.0.0b

+ coming soon!

...stay tuned for more!

For now: the implementation of this eventual mixing plugin is a set of 1st-order filters; high pass, low pass, high shelf, and low shelf. Each filter type has a frequency control (currently 20 - 20,000Hz un-stepped, but subject to change) and the shelves both have a gain control (+/- 30.01dB un-stepped - likely not to change). These are complemented by an output gain control and dry/wet mixer (with processing applied in that order). It is possible that another filter band or two might make it's way into the design before it takes it's final form (possibly to directly address mid-range) along with an optional 2nd-order mode for each band, oversampled non-linearities, and per-band bypass... (lots to be done), but there are several main purposes in mind for this equalizer design, which calls for the 1st-order filter design and which in turn provides only a small set of output characteristic types;

On a musical level;

+ Gentle manipulation of broadband audio material
+ Possibility of "complementary" curves for emphasis/de-emphasis audio processing techniques (think Dolby)
+ Absolutely "spec-perfect" filter centre-frequency behaviour (although currently subject to HF digital cramping - TBA)
+ Mastering-grade processing precision, regardless whether using a Floats or Doubles architecture

On a DSP level;

+ Creation of a robust and durable single-pole "building block" "ideal" filter for laddering, ballistics, smoothing, and furthermore obscure uses
+ Possibility of investigating various de-cramping methods in a more controllable and predictable architecture (being a simple 1-pole structure)
+ Possibility of exploring non-linearities in and around the feedback path
+ Possibility of implementing as an integrator/differentiator for furthermore obscure DSP purposes

*one very interesting technique to explore is to try setting the high and low pass filters at some frequency and then using the two shelves to "equalize" your sound by bringing the highs and lows back to where they were before the filtering. There is precisely enough gain to achieve this in most situations; for example, try setting both the high and low pass filters to 632.455hz (the centre position of the knob, and "logarithmic mean" of 20hz to 20,000hz), the low shelf to 20hz, and the high shelf to 20,000hz. Now boost both of the shelves to full volume (+30.01dB). This should perfectly mitigate the majority of the high and low pass filtering, except for the very corner frequencies (as if you had set the filters to 20hz and 20,000hz but left the shelves at 0dB). This interesting level of parity between filter types and parameter settings is precisely what this digital equalizer aims to explore, and to provide access to as a musical application.*

In the pursuit of a useful musical tool, I hope to further strengthen my current JUCE plugin template and make some strong additions to my DSP codebase that, in conjunction with several of my other existing projects, will open up the path to some very high-quality effects processors which I've already successfully breadboarded using more "stock" parts from the JUCE library. There is a kit of ground to be covered before I can consider an implementation of BiLinearEQ to be "complete", and much of that is not DSP-related at all (indeed most of the DSP is ready to go).

I will try to take some breaks inbetween GUI work and other errands in order to provide some interesting write-ups on the core module at play here - and the many myriad use cases that module has ahead :) - but life is generally busy and I only do this in my spare time, so please stay tuned :)

- Nathan (StoneyDSP) June 2022

# Before you go...

Coffee! That's how I get things done!! If you'd like to see me get more things done, please kindly consider <a href="https://www.patreon.com/bePatron?u=8549187" data-patreon-widget-type="become-patron-button">buying me a coffee</a> or two ;)

<p align="center">
 <a href= "https://paypal.me/StoneyDSPAudio?country.x=ES&locale.x=en_US"><img src="https://www.paypalobjects.com/en_US/i/btn/btn_donate_SM.gif"/></a>
</p>

<p align="center">
 <a href= "https://twitter.com/Stoney_DSP/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/a075caeedffe23b2733ee38b12f9800f62aab9c2/Assets/twitter.png"/></a>
 <a href= "https://www.instagram.com/stoney_dsp/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/2253d684ba99e6c072353a94b49315162c381406/Assets/instagram.png"/></a>
 <a href= "https://www.facebook.com/StoneyDSP.Audio/"><img src="https://github.com/StoneyDSP/StoneyDSP/blob/9608562b09ee2708affd0c31117fc25a235672d9/Assets/facebook.png"/></a>
</p>
