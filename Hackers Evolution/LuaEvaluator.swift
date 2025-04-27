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
    @State var script: String = """
          function fib(m)
            if m < 2 then
              return m
            end
            return fib(m-1) + fib(m-2)
          end

          return fib(15);
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
        } detail: {
            LuaOutput(script: script)
        }
    }
}
