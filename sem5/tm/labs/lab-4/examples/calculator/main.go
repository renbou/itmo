package main

import (
	"fmt"
	"os"
	"strconv"

	"github.com/charmbracelet/bubbles/textinput"
	tea "github.com/charmbracelet/bubbletea"
	"github.com/charmbracelet/lipgloss"
)

var defaultStyle = lipgloss.NewStyle().
	Foreground(lipgloss.Color("#e1ffd4"))

var errStyle = lipgloss.NewStyle().
	Bold(true).
	Foreground(lipgloss.Color("#cc001b"))

type calculator struct {
	oldInputs     []*textinput.Model
	results       []string
	historySelect int
	input         *textinput.Model
}

func newCalculator() *calculator {
	c := &calculator{}
	c.nextInput()

	return c
}

func (c *calculator) nextInput() {
	if c.input != nil {
		c.input.Blur()
		c.oldInputs = append(c.oldInputs, c.input)
	}

	input := textinput.New()
	c.input = &input
	c.input.Focus()
}

func (c *calculator) Init() tea.Cmd {
	return textinput.Blink
}

func (c *calculator) setHistoryInput() {
	c.input.SetValue(c.oldInputs[len(c.oldInputs)-c.historySelect].Value())
	c.input.SetCursor(len(c.input.Value()))
}

func (c *calculator) Update(msg tea.Msg) (tea.Model, tea.Cmd) {
	var cmd tea.Cmd

	switch msg := msg.(type) {
	case tea.KeyMsg:
		switch msg.Type {
		case tea.KeyCtrlC, tea.KeyEsc:
			return c, tea.Quit
		case tea.KeyEnter:
			value, err := calculate(c.input.Value())
			if err != "" {
				c.results = append(c.results, errStyle.Render(err))
			} else {
				c.results = append(c.results, defaultStyle.Render(strconv.FormatFloat(value, 'f', 6, 64)))
			}

			c.nextInput()
			c.historySelect = 0
		case tea.KeyUp:
			if c.historySelect < len(c.oldInputs) {
				c.historySelect++
				c.setHistoryInput()
			}
		case tea.KeyDown:
			if c.historySelect > 0 {
				c.historySelect--

				if c.historySelect == 0 {
					c.input.SetValue("")
				} else {
					c.setHistoryInput()
				}
			}
		}
	}

	*c.input, cmd = c.input.Update(msg)
	return c, cmd
}

func (c *calculator) View() string {
	var history string
	for i := range c.oldInputs {
		history += fmt.Sprintf("%s\n%s\n\n", c.oldInputs[i].View(), c.results[i])
	}

	return history + c.input.View()
}

func calculate(input string) (res float64, msg string) {
	defer func() {
		if r := recover(); r != nil {
			msg = fmt.Sprint(r)
		}
	}()

	e, err := ParseExpr(input)
	if err != nil {
		return 0, fmt.Sprintf("cannot parse expression: %s", err)
	}

	return e.ret, ""
}

func main() {
	p := tea.NewProgram(newCalculator())
	if _, err := p.Run(); err != nil {
		fmt.Printf("Unexpected error: %s\n", err)
		os.Exit(1)
	}
}
