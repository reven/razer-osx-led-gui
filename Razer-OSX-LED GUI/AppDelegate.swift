//
//  AppDelegate.swift
//  Razer-OSX-LED GUI
//
//  Created by Roberto Sanchez on 09/05/2020.
//  Copyright © 2020 Roberto Sanchez. All rights reserved.
//

import Cocoa

// Data model for app variables.

// Struct works better and is accesible everywhere.
struct Razer {
    //First get the nsObject by defining as an optional anyObject
    static var initApply: Bool = false
    static var detected: String = "No device detected"
    static var animation: String = "static"
    static var brightness: Int  = 0
    static var time: Int = 2 // Unused, but part of passable vars
    static var color1: NSColor = NSColor(red: 0.0, green: 1.0, blue: 0.0, alpha: 1.0)
    static var color2: NSColor = NSColor(red: 1.0, green: 0.0, blue: 0.0, alpha: 1.0)
    static var singleColor: Bool = false
    static var ignoreColor: Bool = false
    static var appVersion: String = Bundle.main.infoDictionary?["CFBundleShortVersionString"] as? String ?? "?"
    static var logo: Bool = false
}

@NSApplicationMain
class AppDelegate: NSObject, NSApplicationDelegate {
    
    var statusItemManager: StatusItemManager!


    func applicationDidFinishLaunching(_ aNotification: Notification) {

        // Load our config
        let defaults = UserDefaults.standard
        Razer.initApply = defaults.bool(forKey: "initApply")
        Razer.animation = defaults.string(forKey: "animation") ?? Razer.animation
        Razer.brightness = defaults.integer(forKey: "brightness")
        Razer.color1 = defaults.color(forKey: "color1") ?? Razer.color1
        Razer.color2 = defaults.color(forKey: "color2") ?? Razer.color2
        Razer.singleColor = defaults.bool(forKey: "singleColor")
        Razer.ignoreColor = defaults.bool(forKey: "ignoreColor")
        Razer.logo = defaults.bool(forKey: "logo")
        
        
        // Check if initApplay is true and if so call pollDevice()
        if (Razer.initApply == true) {
            var colorInfo = ""
            if (Razer.ignoreColor != true) {
                let color1r = round( Razer.color1.redComponent * 255)
                let color1g = round( Razer.color1.greenComponent * 255)
                let color1b = round( Razer.color1.blueComponent * 255)
                           
                let color2r = round( Razer.color2.redComponent * 255)
                let color2g = round( Razer.color2.greenComponent * 255)
                let color2b = round( Razer.color2.blueComponent * 255)
                       
                switch Razer.animation {
                case "static":
                    colorInfo = " \(color1r) \(color1g) \(color1b)"
                           
                case "breathe":
                    if (Razer.singleColor == false) {
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
            let cmd = Razer.animation + colorInfo
            pollDevice(cmd)
            let logoTxt = Razer.logo ? "logo on" : "logo off"
            pollDevice(logoTxt)
            pollDevice("brightness \(Razer.brightness)")
            
        }
        
        
        // Insert code here to initialize your application
        statusItemManager = StatusItemManager()
        
    }

    func applicationWillTerminate(_ aNotification: Notification) {
        // Insert code here to tear down your application
    }


}

