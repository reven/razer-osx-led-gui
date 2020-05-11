//
//  ColorDefaults.swift
//  Razer-OSX-LED GUI
//
//  Created by Roberto Sanchez on 10/05/2020.
//  Copyright © 2020 Roberto Sanchez. All rights reserved.
//

import Cocoa

// We extend UserDefaults in order to store NSColor objects, that have to be transformed
// to Data objects first.
// To save a color
//      defaults.set(mycolor, forKey: "mycolor")
// To read a color back. this returns an optional, may be nil
//      defaults.color(forKey: "mycolor")
extension UserDefaults {
    
    func set(_ color: NSColor, forKey: String) {
        if let data = try? NSKeyedArchiver.archivedData(withRootObject: color, requiringSecureCoding: false) {
            self.set(data, forKey: forKey)
        }
    }
    
    func color(forKey: String) -> NSColor? {
        guard
            let storedData = self.data(forKey: forKey),
            let unarchivedData = try? NSKeyedUnarchiver.unarchivedObject(ofClass: NSColor.self, from: storedData),
            let color = unarchivedData as NSColor?
        else {
            return nil
        }
        
        return color
    }
    
}
