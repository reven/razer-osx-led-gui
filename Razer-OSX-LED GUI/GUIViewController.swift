//
//  GUIViewController.swift
//  Razer-OSX-LED GUI
//
//  Created by Roberto Sanchez on 09/05/2020.
//  Copyright © 2020 Roberto Sanchez. All rights reserved.
//

import Cocoa

class GUIViewController: NSViewController {
    
    @IBOutlet weak var detectedTextField: NSTextField!
    
    @IBOutlet weak var colorPicker1: NSColorWell!
    @IBOutlet weak var colorPicker2: NSColorWell!

    @IBOutlet weak var animationTextField: NSPopUpButton!
    
    @IBOutlet weak var singleColor: NSButton!
    @IBOutlet weak var ignoreColor: NSButton!
    
    @IBOutlet weak var brightSlider: NSSlider!
    
    @IBOutlet weak var initApply: NSButton!



    
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do view setup here.
        if let label = pollDevice("info") {
            Razer.detected = String(cString: label)
            detectedTextField.stringValue = Razer.detected
        }
        if let brightness = pollDevice("brightness") {
            // All this to get rid of newline
            let lines = String(cString: brightness).split { $0.isNewline }
            let num = Int (lines[0])
            Razer.brightness = num ?? 0
            brightSlider.integerValue = Razer.brightness
        }
        
        colorPicker1.color = Razer.color1
        colorPicker2.color = Razer.color2
    }
 
    
    // MARK: - IBAction Methods
    
    @IBAction func setAnimation(_ sender: Any){
        
        // Get vars from interface
        let animation = animationTextField.titleOfSelectedItem?.lowercased() ?? "static"
        var colorInfo = ""
        
        if (ignoreColor.state == .off ) {
            
            let color1r = round( colorPicker1.color.redComponent * 255)
            let color1g = round( colorPicker1.color.greenComponent * 255)
            let color1b = round( colorPicker1.color.blueComponent * 255)
            
            let color2r = round( colorPicker2.color.redComponent * 255)
            let color2g = round( colorPicker2.color.greenComponent * 255)
            let color2b = round( colorPicker2.color.blueComponent * 255)
        
            switch animation {
            case "static":
                colorInfo = " \(color1r) \(color1g) \(color1b)"
            
            case "breathe":
                 if (singleColor.state == .off) {
                    colorInfo = " \(color1r) \(color1g) \(color1b)  \(color2r) \(color2g) \(color2b)"
                 } else {
                    colorInfo = " \(color1r) \(color1g) \(color1b)"
                 }
            case "starlight", "reactive":
                colorInfo = " 2 \(color1r) \(color1g) \(color1b)"
                
            case "wave":
                // TO DO: Implement option to change this
                colorInfo = " right"

            default:
                colorInfo = ""
            }
        }
        
        
        let cmd = animation + colorInfo
        pollDevice(cmd)
        
    }
    
    
    @IBAction func setBrightness(_ sender: Any){
        Razer.brightness = brightSlider.integerValue
        pollDevice("brightness \(brightSlider.integerValue)")
    }
    
    
    @IBAction func savePrefs(_ sender: Any){
        let defaults = UserDefaults.standard
        defaults.set(initApply.state == .on ? true : false, forKey: "initApply")
        defaults.set(ignoreColor.state == .on ? true : false, forKey: "ignoreColor")
        defaults.set(singleColor.state == .on ? true : false, forKey: "singleColor")
        defaults.set(animationTextField.titleOfSelectedItem?.lowercased(), forKey: "animation")
        defaults.set(colorPicker1.color, forKey: "color1")
        defaults.set(colorPicker2.color, forKey: "color2")
        defaults.set(brightSlider.integerValue, forKey: "brightness")        
    }
    
    
    @IBAction func showAbout(_ sender: Any) {
        guard let appDelegate = NSApplication.shared.delegate as? AppDelegate, let itemManager = appDelegate.statusItemManager else { return }
        itemManager.showAbout()
    }
    
    
    @IBAction func quit(_ sender: Any) {
        NSApplication.shared.terminate(self)
    }
}
