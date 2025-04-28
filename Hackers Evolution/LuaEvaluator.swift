//
//  LuaEvaluator.swift
//  Hackers Evolution
//
//  Created by Eric Baur on 4/26/25.
//

import SwiftUI
import CodeEditorView
import LanguageSupport

struct LuaEvaluator: View {
//    @State var scriptToExecute: String = ""
    @State var script: String = """
        -- `he` is the Hackers Evolution reference/programming object
        -- he.program can be filled with your program
        -- he.execute() can be used to run it (step-wise?)
        -- he.state gives current program state
        
        he.program = "asdf"
        he.execute()
        
        -- return with other testing stuff
        return he.state, "foo", { one=1, two={ three="foo", four="bar" }}, { 1, 2, 3, 4 }
        """
    @State var position: CodeEditor.Position = .init(selections: [NSMakeRange(0, 0)], verticalScrollPosition: 0)
    @State var messages: Set<TextLocated<Message>> = Set()
    
    @Environment(\.colorScheme) private var colorScheme: ColorScheme
    private var layoutConfiguration: CodeEditor.LayoutConfiguration = CodeEditor.LayoutConfiguration(showMinimap: false, wrapText: true)
    
    var body: some View {
        NavigationSplitView {
            CodeEditor(
                text: $script,
                position: $position,
                messages: $messages
                
            )
            .environment(\.codeEditorLayoutConfiguration, layoutConfiguration)
            .environment(\.codeEditorTheme, (colorScheme == .dark ? Theme.defaultDark : Theme.defaultLight))
            .frame(minWidth: 300)
//            .toolbar {
//                Button("Run query...") {
//                    scriptToExecute = script
//                }.keyboardShortcut("\n")
//            }
        } detail: {
            LuaOutput(script: script)
        }
    }
}
