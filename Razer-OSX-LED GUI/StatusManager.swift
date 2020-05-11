//
//  StatusManager.swift
//  Razer-OSX-LED GUI
//
//  Created by Roberto Sanchez on 09/05/2020.
//  Copyright © 2020 Roberto Sanchez. All rights reserved.
//

import Cocoa

class StatusItemManager: NSObject {
    
    var statusItem: NSStatusItem?
     
    var popover: NSPopover?
     
    var GUIVC: GUIViewController?
    
    fileprivate func initStatusItem() {
        statusItem = NSStatusBar.system.statusItem(withLength: NSStatusItem.squareLength)
        let itemImage = NSImage(named: "circle")
        //itemImage?.isTemplate = true
        statusItem?.button?.image = itemImage
        statusItem?.button?.target = self
        statusItem?.button?.action = #selector(showGUIVC)
    }
    
    @objc fileprivate func showGUIVC() {
        guard let popover = popover, let button = statusItem?.button else { return }
        
        if GUIVC == nil {
            let storyboard = NSStoryboard(name: "Main", bundle: nil)
            guard let vc = storyboard.instantiateController(withIdentifier: .init(stringLiteral: "GUIID")) as? GUIViewController else { return }
            GUIVC = vc
        }
        
        popover.contentViewController = GUIVC
        
        popover.show(relativeTo: button.bounds, of: button, preferredEdge: .minY)
     
    }
            
    fileprivate func initPopover() {
        popover = NSPopover()
        
        // Specify the popover's behavior.
        popover?.behavior = .transient
    }
    
    override init() {
        super.init()
     
        initStatusItem()
        initPopover()
    }
    
    func showAbout() {
        guard let popover = popover else { return }
        
        popover.contentViewController?.view.isHidden = true
     
        let storyboard = NSStoryboard(name: "Main", bundle: nil)
        guard let vc = storyboard.instantiateController(withIdentifier: .init(stringLiteral: "aboutID")) as? AboutViewController else { return }
     
        popover.contentViewController = vc
    }
        
    func hideAbout() {
        guard let popover = popover else { return }
        popover.contentViewController?.view.isHidden = true
        popover.contentViewController?.dismiss(nil)
        showGUIVC()
        popover.contentViewController?.view.isHidden = false
    }
 
}

